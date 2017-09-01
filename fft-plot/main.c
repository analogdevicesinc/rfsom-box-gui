#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <signal.h>
#include <stdio.h>
#include <iio.h>
#include <complex.h>
#include <fftw3.h>
#include <unistd.h>
#include <math.h>
#include "basic_graph.h"
#include <termios.h>

#define FFT_SIZE (8192)
#define IIO_DEVICE_STR "cf-ad9361-lpc"
#define ADC_SCALE ((double)(65535.0))
#define X_RES 160
#define Y_RES 128
#define PIX_SCALE (int)(FFT_SIZE/(double)X_RES)


#define ASSERT(expr) { \
	if (!(expr)) { \
		(void) fprintf(stderr, "assertion failed (%s:%d)\n", __FILE__, __LINE__); \
		(void) abort(); \
	} \
}
static struct iio_device  *dev  =NULL;
static struct iio_context *ctx  = NULL;
static struct iio_channel *rx_i = NULL;
static struct iio_channel *rx_q = NULL;
static struct iio_buffer  *rxbuf = NULL;
static bool stop = false;
fftw_complex in[FFT_SIZE], out[FFT_SIZE];
double win[FFT_SIZE];
fftw_plan p;

static double win_hanning(int j, int n)
{
	double a = 2.0 * M_PI / (n - 1), w;

	w = 0.5 * (1.0 - cos(a * j));

	return (w);
}


#define REAL 0
#define IMAG 1
void shutdown();

// http://cc.byexamples.com/2007/04/08/non-blocking-user-input-in-loop-without-ncurses/
#define NB_ENABLE 1
#define NB_DISABLE 0
int kbhit()
{
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds); //STDIN_FILENO is 0
	select(STDIN_FILENO+1, &fds, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &fds);
}

void nonblock(int state)
{
	struct termios ttystate;

	//get the terminal state
	tcgetattr(STDIN_FILENO, &ttystate);

	if (state==NB_ENABLE) {
		//turn off canonical mode
		ttystate.c_lflag &= ~ICANON;
		//minimum of number input read.
		ttystate.c_cc[VMIN] = 1;
	} else if (state==NB_DISABLE) {
		//turn on canonical mode
		ttystate.c_lflag |= ICANON;
	}

	//set the terminal attributes.
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);

}

static void handle_sig(int sig)
{
	printf("Waiting for process to finish...\n");
	stop = true;
}
static double limit_d(double val, double min, double max)
{
	if (val>max) {
		return max;
	}

	if (val<min) {
		return min;
	}

	return val;
}
double round(double d)
{
	return floor(d + 0.5);
}
int map(int output_start, int output_end, double input, double input_start,
        double input_end)
{
// https://stackoverflow.com/questions/5731863/mapping-a-numeric-range-onto-another
	double slope = 1.0 * (output_end - output_start) / (input_end - input_start);
	int output = output_start + round(slope * (input - input_start));

	if (output>output_end) {
		return output_end;
	}

	if (output<output_start) {
		return output_start;
	}

	return output;
}

int main(int argc, char **argv)
{
	int pfd;

	if (argc>1) {
		char *filename = argv[1];

		if ((pfd = open(filename, O_WRONLY | O_CREAT | O_TRUNC,
		                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)) == -1) {
			perror("Cannot open output file\n");
			exit(1);
			pfd = STDOUT_FILENO;
		}
	} else {
		pfd = STDOUT_FILENO;
	}

	FILE *fp = fdopen(pfd, "w");


	signal(SIGINT, handle_sig);
	printf("* Acquiring IIO context\n");
	ASSERT((ctx = iio_create_default_context()) && "No context");

	printf("* Acquiring AD9361 streaming devices\n");
	dev = iio_context_find_device(ctx,IIO_DEVICE_STR);

	printf("* Initializing AD9361 IIO streaming channels\n");
	rx_i = iio_device_find_channel(dev, "voltage0", 0); // INPUT
	rx_q = iio_device_find_channel(dev, "voltage1", 0); // INPUT

	printf("* Enabling IIO streaming channels\n");
	iio_channel_enable(rx_i);
	iio_channel_enable(rx_q);

	printf("* Creating non-cyclic IIO buffers with %d samples\n",FFT_SIZE);
	rxbuf = iio_device_create_buffer(dev, FFT_SIZE, false);

	if (!rxbuf) {
		perror("Could not create RX buffer");
		shutdown();
	}

	printf("* Starting IO streaming (press CTRL+C to cancel)\n");

//    	in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);
//    	out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * FFT_SIZE);
	p = fftw_plan_dft_1d(FFT_SIZE, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
	init_fb();
	clear_screen(0);
	double min[X_RES];
	double max[X_RES];
	int minpix[X_RES];
	int maxpix[X_RES];

	int cnt=0;
	int i;

	for (i = 0; i < FFT_SIZE; i++) {
		win[i] = win_hanning(i, FFT_SIZE);
	}

	for (cnt=0; cnt<X_RES; cnt++) {
		minpix[cnt]=maxpix[cnt]=0;
	}

	nonblock(NB_ENABLE);

	while (!stop) {
		ssize_t nbytes_rx, nbytes_tx;
		void *p_dat, *p_end;
		ptrdiff_t p_inc;

		nbytes_rx = iio_buffer_refill(rxbuf);

		if (nbytes_rx < 0) {
			printf("Error refilling buf %d\n",(int) nbytes_rx);
			shutdown();
		}

		p_inc = iio_buffer_step(rxbuf);
		p_end = iio_buffer_end(rxbuf);
		cnt=0;

		for (p_dat = iio_buffer_first(rxbuf, rx_i); p_dat < p_end;
		     p_dat += p_inc, cnt++) {
			// Example: swap I and Q

			const int16_t real = ((int16_t *)p_dat)[0];
			const int16_t imag = ((int16_t *)p_dat)[1];
			in[cnt] = (real * win[cnt] + I * imag * win[cnt])/ 2048;
		}

		fftw_execute(p); /* repeat as needed */

		for (cnt=0; cnt<X_RES; cnt++) {
			min[cnt]=1000;
			max[cnt]=-1000;
		}

		for (cnt=0; cnt<FFT_SIZE; cnt++) {
			double ampl = 10 * log10((creal(out[cnt]) * creal(out[cnt]) + cimag(
			                                  out[cnt]) * cimag(out[cnt])) /
			                         ((unsigned long long)FFT_SIZE * (unsigned long long)FFT_SIZE));

			if (ampl>max[cnt/PIX_SCALE]) {
				max[cnt/PIX_SCALE]=ampl;
			}

			if (ampl<min[cnt/PIX_SCALE]) {
				min[cnt/PIX_SCALE]=ampl;
			}
		}

		double global_min=-140;
		double global_max=0;
		int pix_start=0;
		int pix_end=125;

		for (cnt=0; cnt<=X_RES/2; cnt++) {
			draw_line(cnt+X_RES/2,minpix[cnt],cnt+X_RES/2,maxpix[cnt],0x00);
			minpix[cnt] = map(pix_start,pix_end,min[cnt],global_max,global_min);
			maxpix[cnt] = map(pix_start,pix_end,max[cnt],global_max,global_min);
			draw_line(cnt+X_RES/2,minpix[cnt],cnt+X_RES/2,maxpix[cnt],0xff);
		}

		for (cnt=X_RES/2; cnt<X_RES; cnt++) {
			draw_line(cnt-X_RES/2,minpix[cnt],cnt-X_RES/2,maxpix[cnt],0x00);
			minpix[cnt] = map(pix_start,pix_end,min[cnt],global_max,global_min);
			maxpix[cnt] = map(pix_start,pix_end,max[cnt],global_max,global_min);
			draw_line(cnt-X_RES/2,minpix[cnt],cnt-X_RES/2,maxpix[cnt],0xff);
		}

		uint16_t axis_color=get_color(232,232,224);
		draw_line(0,0,5,0,axis_color);
		draw_line(0,0,0,127,axis_color);
		draw_line(0,127,5,127,axis_color);
		draw_text("-140db",2,112,axis_color);
		draw_text(" 0db",2,2,axis_color);

		if (kbhit()) {
			char c=fgetc(stdin);

			if (c=='q') {
				stop=1;
			}
		}
	}

	if (pfd!=STDOUT_FILENO) {
		close(pfd);
	}

	shutdown();
	return 0;
}

void shutdown()
{
	printf("* Destroying buffers\n");

	if (rxbuf) {
		iio_buffer_destroy(rxbuf);
	}

	printf("* Disabling streaming channels\n");

	if (rx_i) {
		iio_channel_disable(rx_i);
	}

	if (rx_q) {
		iio_channel_disable(rx_q);
	}

	printf("* Destroying context\n");

	if (ctx) {
		iio_context_destroy(ctx);
	}

	fftw_destroy_plan(p);
	//	fftw_free(in); fftw_free(out);
	nonblock(NB_DISABLE);
	deinit_fb();
	exit(0);
}
