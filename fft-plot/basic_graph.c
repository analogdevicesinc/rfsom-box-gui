#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include "basic_graph.h"
#include "basic_font.h"
//extern uint8_t **font;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;
static long int screensize = 0;
static char *fbp = 0;
static int fbfd = 0;

/*static unsigned long devMem = open("/dev/mem",   O_RDWR);
static volatile unsigned long *gp2x_memregl=(unsigned long *)mmap(0, 0x10000, PROT_READ|PROT_WRITE, MAP_SHARED, devMem, 0xc0000000);

static volatile unsigned short *gp2x_memregs=(unsigned short *)gp2x_memregl;*/
#define MAX(a,b) ({ \
    typeof(a) _a_temp_; \
    typeof(b) _b_temp_; \
    _a_temp_ = (a); \
    _b_temp_ = (b); \
    _a_temp_ = _a_temp_ < _b_temp_ ? _b_temp_ : _a_temp_; \
    })

#define MIN(a,b) ({ \
    typeof(a) _a_temp_; \
    typeof(b) _b_temp_; \
    _a_temp_ = (a); \
    _b_temp_ = (b); \
    _a_temp_ = _a_temp_ > _b_temp_ ? _b_temp_ : _a_temp_; \
    })


struct fb_var_screeninfo *get_vinfo()
{
	return &vinfo;
}
#define get_blue(color) (color&(0x1f));
#define get_red(color)  (color&(0xf100))>>16;
#define get_green(color) (color&(0x07e0))>>8;

uint16_t get_color(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t newr = (r * 0x1f) / 0xff;
	uint16_t newg = (g * 0x3f) / 0xff;
	uint16_t newb = (b * 0x1f) / 0xff;
	return (newr << 11)|(newg << 6) |(newb);

}

void draw(int x, int y, uint16_t color)
{
	long int location = (x+vinfo.xoffset) * (vinfo.bits_per_pixel/8) +
	                    (y+vinfo.yoffset) * finfo.line_length;
	unsigned short int t = color; //r<<11 | g << 5 | b;
	*((unsigned short int *)(fbp + location)) = t;

}

void draw_line(int x1, int y1, int x2, int y2, uint16_t pixel)
{
	int i,dx,dy,sdx,sdy,dxabs,dyabs,x,y,px,py;

	dx=x2-x1;			//Delta x
	dy=y2-y1;			//Delta y
	dxabs=abs(dx);		//Absolute delta
	dyabs=abs(dy);		//Absolute delta
	sdx=(dx>0)?1:-1; //signum function
	sdy=(dy>0)?1:-1; //signum function
	x=dyabs>>1;
	y=dxabs>>1;
	px=x1;
	py=y1;

	if (dxabs>=dyabs) {
		for (i=0; i<dxabs; i++) {
			y+=dyabs;

			if (y>=dxabs) {
				y-=dxabs;
				py+=sdy;
			}

			px+=sdx;
			draw(px,py,pixel);
		}
	} else {
		for (i=0; i<dyabs; i++) {
			x+=dxabs;

			if (x>=dyabs) {
				x-=dyabs;
				px+=sdx;
			}

			py+=sdy;
			draw(px,py,pixel);
		}
	}
}

void draw_char(char character, int x, int y, uint16_t color)
{
	int i,j;
	int index = character-fontarray_offset;

	if (index<0 || index>fontarray_size) {
		index=fontarray_size;
	}

	for (i=0; i<8; i++)
		for (j=0; j<font_height; j++) {
			if (font[index][font_height-j]&(1<<i)) {
				draw(x+8-i,y+j,color);
			}
		}
}

void draw_text(char *text,int x,int y, uint16_t color)
{
	while (*text) {
		draw_char((*text),x, y, color);
		text++;
		x+=8;
	}
}

void clear_screen(int color)
{
	for (int y = 0; y < vinfo.yres; y++)
		for (int x = 0; x < vinfo.xres; x++) {
			draw(x,y,color);
		}

}

void wait_vsync()
{
	/*	while(gp2x_memregs[0x1182>>1]&(1<<4));
		while(!(gp2x_memregs[0x1182>>1]&(1<<4)));*/
}

void init_fb()
{
	int x = 0, y = 0;
	long int location = 0;

	// Open the file for reading and writing
	fbfd = open("/dev/fb0", O_RDWR);

	if (fbfd == -1) {
		perror("Error: cannot open framebuffer device");
		exit(1);
	}

	printf("The framebuffer device was opened successfully.\n");

	// Get fixed screen information
	if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
		perror("Error reading fixed information");
		exit(2);
	}

	// Get variable screen information
	if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
		perror("Error reading variable information");
		exit(3);
	}

	printf("%dx%d, %dbpp\n", vinfo.xres, vinfo.yres, vinfo.bits_per_pixel);

	// Figure out the size of the screen in bytes
	screensize = vinfo.xres * vinfo.yres * vinfo.bits_per_pixel / 8;

	// Map the device to memory
	fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED,
	                   fbfd, 0);

	if ((int)fbp == -1) {
		perror("Error: failed to map framebuffer device to memory");
		exit(4);
	}

	printf("The framebuffer device was mapped to memory successfully.\n");
}

void deinit_fb()
{
	munmap(fbp, screensize);
	close(fbfd);
}


