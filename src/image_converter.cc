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

int convert_nv12_to_bgra_rga(char *src, char *dst, int width, int height) {
  int ret = 0;
  int src_width, src_height, src_format;
  int dst_width, dst_height, dst_format;
  int src_buf_size, dst_buf_size;
  rga_buffer_t src_img, dst_img;
  rga_buffer_handle_t src_handle, dst_handle;

  memset(&src_img, 0, sizeof(src_img));
  memset(&dst_img, 0, sizeof(dst_img));

  src_width = align_to_16(width);
  src_height = height;
  src_format = RK_FORMAT_YCbCr_420_SP;

  dst_width = align_to_16(height);
  dst_height = width;
  dst_format = RK_FORMAT_BGRA_8888;

  src_buf_size = src_width * src_height * 3 / 2;
  dst_buf_size = dst_width * dst_height * 4;

  src_handle = importbuffer_virtualaddr(src, src_buf_size);
  dst_handle = importbuffer_virtualaddr(dst, dst_buf_size);

  // 包装缓冲区
  src_img = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
  dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

  // 执行图像检查
  ret = imcheck(src_img, dst_img, {}, {});
  if (IM_STATUS_NOERROR != ret) {
    printf("%d, check error! %s", __LINE__, imStrError((IM_STATUS)ret));
    goto release_buffer;
  }
  // 执行图像旋转并转格式
  ret = imrotate(src_img, dst_img, IM_HAL_TRANSFORM_ROT_90);
  if (ret == IM_STATUS_SUCCESS) {
    // printf("imrotate running success!\n");
  } else {
    printf("imrotate running failed, %s\n", imStrError((IM_STATUS)ret));
    goto release_buffer;
  }

release_buffer:
  if (src_handle)
    releasebuffer_handle(src_handle);
  if (dst_handle)
    releasebuffer_handle(dst_handle);

  return ret;
}

int convert_nv12_to_bgra_dma_buf(char *src, My_drm_context_t *drm, int width,
                                 int height) {
  int ret = 0;
  int src_width, src_height, src_format;
  int dst_width, dst_height, dst_format;
  int src_buf_size, dst_buf_size;
  rga_buffer_t src_img, dst_img;
  rga_buffer_handle_t src_handle, dst_handle;

  // 检查DMA-BUF fd是否有效
  if (drm->dma_buf_fd < 0) {
    fprintf(stderr, "Invalid DMA-BUF fd, falling back to normal mode\n");
    // 调用普通转换函数
    return -1;
  }

  memset(&src_img, 0, sizeof(src_img));
  memset(&dst_img, 0, sizeof(dst_img));

  src_width = align_to_16(width); //16对齐很重要
  src_height = height;
  src_format = RK_FORMAT_YCbCr_420_SP; // NV12格式

  dst_width = align_to_16(drm->width); //16对齐很重要
  dst_height = drm->height;
  dst_format = RK_FORMAT_BGRA_8888; // BGRA格式

  src_buf_size = src_width * src_height * 3 / 2;
  dst_buf_size = dst_width * dst_height * 4;

  src_handle = importbuffer_virtualaddr(src, src_buf_size);
  // 通过DMA-BUF fd导入DRM缓冲区到RGA
  dst_handle = importbuffer_fd(drm->dma_buf_fd, dst_buf_size);

  if (!dst_handle) {
    perror("Failed to import DMA-BUF to RGA");
    releasebuffer_handle(src_handle);
    return -1;
  }

  // 包装缓冲区
  src_img = wrapbuffer_handle(src_handle, src_width, src_height, src_format);
  dst_img = wrapbuffer_handle(dst_handle, dst_width, dst_height, dst_format);

  // 执行图像检查
  ret = imcheck(src_img, dst_img, {}, {});
  if (IM_STATUS_NOERROR != ret) {
    printf("Line %d, check error! %s\n", __LINE__, imStrError((IM_STATUS)ret));
    goto release_buffer;
  }

  // 执行图像旋转并转格式
  ret = imrotate(src_img, dst_img, IM_HAL_TRANSFORM_ROT_90);
  if (ret != IM_STATUS_SUCCESS) {
    printf("imrotate running failed, %s\n", imStrError((IM_STATUS)ret));
  }

release_buffer:
  if (src_handle)
    releasebuffer_handle(src_handle);
  if (dst_handle)
    releasebuffer_handle(dst_handle);
  return ret;
}
