#include <cstdio>
//#include <rga/im2d_single.h>
#include "image_converter.h"
#include <rga/RgaApi.h>
#include <rga/RgaUtils.h>
#include <rga/im2d.h>
#include <stdlib.h>
#include <string.h>

#define LOCAL_FILE_PATH "/data"

static inline size_t align_to_16(size_t size) { return (size + 15) & ~15; }

int convert_nv12_to_RGB(char *src_data, char *dst_data, int width, int height) {
  int ret = 0;
  int src_width, src_height, src_format;
  int dst_width, dst_height, dst_format;

  rga_buffer_t src = {0};
  rga_buffer_t dst = {0};

  
  src_width = width;
  src_height = height;
  src_format = RK_FORMAT_YCbCr_420_SP;

  dst_width = width;
  dst_height = height;
  dst_format = RK_FORMAT_RGB_888;

  src = wrapbuffer_virtualaddr(src_data, src_width, src_height, src_format);
  dst = wrapbuffer_virtualaddr(dst_data, dst_width, dst_height, dst_format);

  ret = imcvtcolor(src, dst, src_format, dst_format);
  if (ret == IM_STATUS_SUCCESS) {
    // printf("imrotate running success!\n");
  } else {
    printf("imcvtcolor running failed, %s\n", imStrError((IM_STATUS)ret));
  }

  return ret;
}

int convert_RGB_to_BGRA_dma_buf(char *src_data, My_drm_context_t *drm, int width,
                                 int height) {
  int ret = 0;
  int src_width, src_height, src_format;
  int dst_width, dst_height, dst_format;
  rga_buffer_t src = {0};
  rga_buffer_t dst = {0};

  // 检查DMA-BUF fd是否有效
  if (drm->dma_buf_fd < 0) {
    fprintf(stderr, "Invalid DMA-BUF fd, falling back to normal mode\n");
    // 调用普通转换函数
    return -1;
  }

  src_width = align_to_16(width); //16对齐很重要
  src_height = height;
  src_format = RK_FORMAT_RGB_888; // RGB格式

  dst_width = align_to_16(drm->width); //16对齐很重要
  dst_height = drm->height;
  dst_format = RK_FORMAT_BGRA_8888; // BGRA格式

  src = wrapbuffer_virtualaddr(src_data, src_width, src_height, src_format);
  // 通过DMA-BUF fd导入DRM缓冲区到RGA
  dst = wrapbuffer_fd(drm->dma_buf_fd, dst_width, dst_height, dst_format);

  // 执行图像旋转并转格式
  ret = imrotate(src, dst, IM_HAL_TRANSFORM_ROT_90);
  if (ret != IM_STATUS_SUCCESS) {
    printf("imrotate running failed, %s\n", imStrError((IM_STATUS)ret));
  }

  return ret;

}


int convert_color(char *src_data, char *dst_data, int width, int height , int src_format, int dst_format) {

  int ret = 0;

  rga_buffer_t src = {0};
  rga_buffer_t dst = {0};


  src = wrapbuffer_virtualaddr(src_data, width, height, src_format);
  dst = wrapbuffer_virtualaddr(dst_data, width, height, dst_format);

  ret = imcvtcolor(src, dst, src_format, dst_format);
  if (ret == IM_STATUS_SUCCESS) {
    // printf("imrotate running success!\n");
  } else {
    printf("imcvtcolor running failed, %s\n", imStrError((IM_STATUS)ret));
  }

  return ret;
}
