#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <linux/fb.h>

typedef struct lcd_color {
    unsigned char bule;
    unsigned char green;
    unsigned char red;
    unsigned char alpha;
} lcd_color;

unsigned char *fb_init(void);
void fb_exit(unsigned char *fbp);
void screen_refresh(unsigned char *fbp, lcd_color color_buff, long screen_size);
extern int fp;

extern int rgb_type;
extern long screen_size;
extern struct fb_var_screeninfo vinfo;
extern struct fb_fix_screeninfo finfo;

#endif /* FRAMEBUFFER_H */