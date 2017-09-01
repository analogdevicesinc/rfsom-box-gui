#ifndef BASIC_GRAPH_H
#define BASIC_GRAPH_H
#include <stdint.h>
#include <linux/fb.h>

struct fb_var_screeninfo *get_vinfo();
uint16_t get_color(uint8_t r, uint8_t g, uint8_t b);
void draw(int x, int y, uint16_t color);
void draw_line(int x1, int y1, int x2, int y2, uint16_t pixel);
void draw_char(char character, int x, int y, uint16_t color);
void draw_text(char *text, int x, int y, uint16_t color);
void clear_screen(int color);
void init_fb();
void deinit_fb();
void wait_vsync();
#endif
