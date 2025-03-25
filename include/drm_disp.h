#ifndef DRM_DISP_H
#define DRM_DISP_H

#include <xf86drm.h>
#include <xf86drmMode.h>
#include <drm_fourcc.h>

typedef struct {
    int fd;
    uint32_t connector_id;
    uint32_t crtc_id;
    uint32_t plane_id;
    drmModeModeInfo mode;
    uint32_t fb_id;
    struct drm_mode_create_dumb create_dumb;
    struct drm_mode_map_dumb map_dumb;
    uint8_t *map_addr;
    int dma_buf_fd;
    uint32_t width;
    uint32_t height;
} My_drm_context_t;

My_drm_context_t* init_drm(int width, int height);
void destroy_drm(My_drm_context_t *drm);

#endif /* DRM_DISP_H */