#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>

#include "framebuffer.h"

int fp = 0;
unsigned char *fbp = 0;
int rgb_type = 0;
long screen_size = 0; 
struct fb_var_screeninfo vinfo;
struct fb_fix_screeninfo finfo;

void screen_refresh(unsigned char *fbp, lcd_color color_buff, long screen_size)
{
    for(int i=0; i < screen_size; i+=4)
    {
        *((lcd_color*)(fbp + i)) = color_buff;
    }
    usleep(1000*2000);
}

unsigned char * fb_init(void)
{
    fp = open("/dev/fb0", O_RDWR);
 
    if (fp < 0)
    {
        printf("Error : Can not open framebuffer device/n");
        exit(1);
    }
 
    if (ioctl(fp, FBIOGET_FSCREENINFO, &finfo))
    {
        printf("Error reading fixed information/n");
        exit(2);
    }
 
    if (ioctl(fp, FBIOGET_VSCREENINFO, &vinfo))
    {
        printf("Error reading variable information/n");
        exit(3);
    }
 
    /* 打印获取的屏幕信息 */
    printf("The mem is :%d\n", finfo.smem_len);
    printf("The line_length is :%d\n", finfo.line_length);
    printf("The xres is :%d\n", vinfo.xres);
    printf("The yres is :%d\n", vinfo.yres);
    printf("bits_per_pixel is :%d\n", vinfo.bits_per_pixel);
 
    /* 获取RGB的颜色颜色格式，比如RGB8888、RGB656 */
    rgb_type = vinfo.bits_per_pixel / 8;
    /* 屏幕的像素点 */
    screen_size = vinfo.xres * vinfo.yres * rgb_type;
    /* 映射 framebuffer 的缓冲空间，得到一个指向这块空间的指针 */
    fbp = (unsigned char *) mmap(NULL, screen_size, PROT_READ | PROT_WRITE, MAP_SHARED, fp, 0);
    if (fbp == NULL)
    {
       printf("Error: failed to map framebuffer device to memory./n");
       exit(4);
    }
    
    // screen_refresh(fbp, (lcd_color){0, 0, 255, 255}, screen_size);
    // usleep(1000*2000);

    
    return fbp;
}

void fb_exit(unsigned char *fbp)
{
    /* 解除映射 */
    munmap(fbp, screen_size); 
    close(fp);
}