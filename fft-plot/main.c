/*
 *  Copyright (c) 2017 Analog Devices
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#define _GNU_SOURCE

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
#include <string.h>
#include <locale.h>
#include <dirent.h>

#include <linux/input.h>

#ifndef EV_SYN
#define EV_SYN 0
#endif
#define DEV_INPUT_EVENT "/dev/input"
#define EVENT_DEV_NAME "event"

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
static struct iio_device  *phy  =NULL;
static struct iio_context *ctx  = NULL;
static struct iio_channel *rx_i = NULL;
static struct iio_channel *rx1  = NULL;
static struct iio_channel *rx_q = NULL;
static struct iio_channel *rx_lo = NULL;
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

/* next two functions are borrowed from:
 * https://cgit.freedesktop.org/evtest/tree/evtest.c
 * Copyright (c) 1999-2000 Vojtech Pavlik
 * Copyright (c) 2009-2011 Red Hat, Inc
 * released under GNU General Public License 2, or any later version
 */

/**
 * Filter for the AutoDevProbe scandir on /dev/input.
 *
 * @param dir The current directory entry provided by scandir.
 *
 * @return Non-zero if the given directory entry starts with "event", or zero
 * otherwise.
 */
static int is_event_device(const struct dirent *dir) {
	return strncmp(EVENT_DEV_NAME, dir->d_name, 5) == 0;
}

/**
 * Scans all /dev/input/event*, display them and ask the user which one to
 * open.
 *
 * @return The event device file name of the device file selected. This
 * string is allocated and must be freed by the caller.
 */
static char* scan_devices(void)
{
	struct dirent **namelist;
	int i, ndev, devnum;
	char *filename;
	int max_device = 0;

	ndev = scandir(DEV_INPUT_EVENT, &namelist, is_event_device, versionsort);
	if (ndev <= 0)
		return NULL;

	fprintf(stderr, "Available devices:\n");

	for (i = 0; i < ndev; i++)
	{
		char fname[64];
		int fd = -1, rc;
		char name[256] = "???";
		unsigned long evbit = 0;
		size_t nchar = KEY_MAX/8 + 1;
		unsigned char bits[nchar];

		snprintf(fname, sizeof(fname),
			 "%s/%s", DEV_INPUT_EVENT, namelist[i]->d_name);

		fd = open(fname, O_RDONLY);
		if (fd < 0)
			continue;

		ioctl(fd, EVIOCGBIT(0, sizeof(evbit)), &evbit);
		if ((evbit & (1 << EV_KEY)) != (1 << EV_KEY)) {
			fprintf(stderr, "%s: skipping, does not report any EV_KEY events.\n", fname);
			close(fd);
			free(namelist[i]);
			continue;
		}

		ioctl(fd, EVIOCGBIT(0, sizeof(bits)), &bits);
		if (!bits[KEY_Q/8] & (1 << (KEY_Q % 8))) {
			fprintf(stderr, "%s: skipping, does not support KEY_Q\n", fname);
			close(fd);
			free(namelist[i]);
			continue;
		}

		ioctl(fd, EVIOCGNAME(sizeof(name)), name);

		/* try to grab it */
		rc = ioctl(fd, EVIOCGRAB, (void*)1);
		if (rc == 0) {
			ioctl(fd, EVIOCGRAB, (void*)0);
		} else {
			fprintf(stderr, "%s: skipping, is grabbed by another process\n", fname);
			close(fd);
			free(namelist[i]);
			continue;
		}

		close(fd);
		fprintf(stderr, "%s:    %s\n", fname, name);

		sscanf(namelist[i]->d_name, "event%d", &devnum);
		if (devnum > max_device)
			max_device = devnum;

		free(namelist[i]);
		asprintf(&filename, "%s/%s%d",
			DEV_INPUT_EVENT, EVENT_DEV_NAME, devnum);

		break;
	}
	if (devnum > max_device || devnum < 0)
		return NULL;

	return filename;
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

void change_lo(long long *lo, long long increment)
{
	printf("%lli\n", *lo);
	*lo = *lo + increment;
	if (*lo < 70e6)
		*lo = 70e6;
	if (*lo > 6e9)
		*lo = 6e9;
	iio_channel_attr_write_longlong(rx_lo, "frequency", *lo);

}

int main(int argc, char **argv)
{
	int pfd;
	char buf[128], *keyboard = NULL;
	int ret;
	long long lo = 70e6, sampling_freq;
	int kfd;
	struct input_event ev[64];
	fd_set rdfs;
	struct timeval dontwait;

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

	keyboard = scan_devices();
	kfd = open(keyboard, O_RDONLY | O_NONBLOCK);
	if (kfd < 0) {
		free(keyboard);
		keyboard = NULL;
	}
	FD_ZERO(&rdfs);
	FD_SET(kfd, &rdfs);
	dontwait.tv_sec = 0;
	dontwait.tv_usec = 0;


	signal(SIGINT, handle_sig);
	printf("* Acquiring IIO context\n");
	ASSERT((ctx = iio_create_default_context()) && "No context");

	printf("* Acquiring AD9361 streaming devices\n");
	dev = iio_context_find_device(ctx,IIO_DEVICE_STR);
	if (!dev)
		exit(1);
	phy = iio_context_find_device(ctx, "ad9361-phy");
	if (!phy)
		exit(1);

	printf("* Initializing AD9361 IIO streaming channels\n");
	rx_i = iio_device_find_channel(dev, "voltage0", 0); // INPUT
	rx_q = iio_device_find_channel(dev, "voltage1", 0); // INPUT
	rx1 =  iio_device_find_channel(phy, "voltage0", 0); // INPUT
	iio_channel_attr_read_longlong(rx1, "sampling_frequency", &sampling_freq);
	/* Don't set the sample rate, incase the modem is running, since that messes it up
	 * iio_channel_attr_write_longlong(rx1, "sampling_frequency", (long long)(30.72e6));
	 */
	printf("sampling rate = %f\n", (double)sampling_freq/1e6);
	rx_lo =  iio_device_find_channel(phy, "RX_LO", 1); // OUTPUT
	ret = iio_channel_attr_read_longlong(rx_lo, "frequency", &lo);

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
	setlocale(LC_NUMERIC, "");

	while (!stop) {
		ssize_t nbytes_rx, nbytes_tx;
		void *p_dat, *p_end;
		ptrdiff_t p_inc;
		double gain;
		unsigned int keypress = 0;

		if (kfd) {
			select(kfd + 1, &rdfs, NULL, NULL, &dontwait);
			ret = read(kfd, ev, sizeof(ev));
			if (ret > 0) {
				for (i = 0; i < ret / sizeof(struct input_event); i++) {
					if (ev[i].type != EV_KEY)
						continue;
					if (ev[i].value == 0)
						continue;
					keypress = ev[i].code;
				}
			}
		}

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
		draw_text(" 0dB",2,2,axis_color);

		switch (keypress) {
		case KEY_Q:
			change_lo(&lo, 1e9);
			break;
		case KEY_A:
			change_lo(&lo, -1e9);
			break;
		case KEY_W:
			change_lo(&lo, 1e8);
			break;
		case KEY_S:
			change_lo(&lo, -1e8);
			break;
		case KEY_E:
			change_lo(&lo, 1e7);
			break;
		case KEY_D:
			change_lo(&lo, -1e7);
			break;
		case KEY_R:
			change_lo(&lo, 1e6);
			break;
		case KEY_F:
			change_lo(&lo, -1e6);
			break;
		case KEY_T:
			change_lo(&lo, 1e5);
			break;
		case KEY_G:
			change_lo(&lo, -1e5);
			break;
		case 0:
		default:
			break;
		}

		sprintf(buf, "-");
		iio_channel_attr_read(rx1, "rssi", &buf[1], sizeof(buf) -1);
		for (cnt=2; cnt <= 13; cnt++)
			draw_line(80, cnt, X_RES - 1, cnt,0x00);
		draw_text(buf,80,2, axis_color);

		iio_channel_attr_read_double(rx1, "hardwaregain", &gain);
		sprintf(buf, "%2.1f GAIN", gain);
		for (cnt=14; cnt <= 26; cnt++)
			draw_line(80, cnt, X_RES - 1, cnt,0x00);
		draw_text(buf,80,14, axis_color);

		draw_text("-140dB",2,112,axis_color);

		sprintf(buf, "%0.4f GHz", (double)lo/1e9 );
		for (cnt=112; cnt <= 112 + 13; cnt++)
			 draw_line(70, cnt, X_RES - 1, cnt,0x00);
		draw_text(buf,70,112, axis_color);

		if (kbhit()) {
			char c=fgetc(stdin);
			switch(c)
			{
			case 'q':
				stop=1;
				break;
			case 'Q':
				change_lo(&lo, 1e9);
				break;
			case 'A':
				change_lo(&lo, -1e9);
				break;
			case 'W':
				change_lo(&lo, 1e8);
				break;
			case 'S':
				change_lo(&lo, -1e8);
				break;
			case 'E':
				change_lo(&lo, 1e7);
				break;
			case 'D':
				change_lo(&lo, -1e7);
				break;
			case 'R':
				change_lo(&lo, 1e6);
				break;
			case 'F':
				change_lo(&lo, -1e6);
				break;
			case 'T':
				change_lo(&lo, 1e5);
				break;
			case 'G':
				change_lo(&lo, -1e5);
				break;
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
