#ifndef IMAGE_CONVERTER_H
#define IMAGE_CONVERTER_H

#include <cstdint>
#include "drm_disp.h"
int convert_nv12_to_RGB(char *src, char *dst, int width, int height);
int convert_RGB_to_BGRA_dma_buf(char *src, My_drm_context_t *drm, int width, int height);
int convert_color(char *src, char *dst, int width, int height, int src_format, int dst_format);
#endif /* IMAGE_CONVERTER_H */