#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "drm_disp.h"
#include <climits>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

// Initialize DRM for landscape mode
My_drm_context_t* init_drm(int width, int height) {
    My_drm_context_t *drm = (My_drm_context_t*)malloc(sizeof(My_drm_context_t));
    if (!drm) {
        perror("Failed to allocate DRM context");
        return NULL;
    }
    
    drm->width = width;  // Original height becomes width in landscape
    drm->height = height;  // Original width becomes height in landscape
    
    // Open DRM device
    drm->fd = open("/dev/dri/card0", O_RDWR | O_CLOEXEC);
    if (drm->fd < 0) {
        perror("Failed to open DRM device");
        free(drm);
        return NULL;
    }
    
    // Get resources
    drmModeRes *res = drmModeGetResources(drm->fd);
    if (!res) {
        perror("Failed to get DRM resources");
        close(drm->fd);
        free(drm);
        return NULL;
    }
    
    //打印资源具体ID信息

    // printf("DRM resources:\n");
    // printf("  Connectors: %d\n", res->count_connectors);
    // printf("  CRTCs: %d\n", res->count_crtcs);
    // printf("  Encoders: %d\n", res->count_encoders);
    // printf("  Framebuffers: %d\n", res->count_fbs);
    // //打印所有CRTCs ID
    // for (int i = 0; i < res->count_crtcs; i++) {
    //     printf("  CRTC %d: %d\n", i, res->crtcs[i]);
    // }
    // //打印所有CONN ID
    // for (int i = 0; i < res->count_connectors; i++) {
    //     printf("  Connector %d: %d\n", i, res->connectors[i]);
    // }
    
    // Find the DSI-1 connector specifically
    drmModeConnector *conn = NULL;
    for (int i = 0; i < res->count_connectors; i++) {
        conn = drmModeGetConnector(drm->fd, res->connectors[i]);
        if (conn->connection == DRM_MODE_CONNECTED) {
            // Check if this is DSI-1
            const char *connector_name = drmModeGetConnectorTypeName(conn->connector_type);
            if (conn->connector_type == DRM_MODE_CONNECTOR_DSI) {
                drm->connector_id = conn->connector_id;
                
                // Find the mode closest to our desired landscape dimensions
                int best_mode = -1;
                int best_mode_diff = INT_MAX;
                
                for (int m = 0; m < conn->count_modes; m++) {
                    // We want landscape mode where width > height
                    if (conn->modes[m].hdisplay > conn->modes[m].vdisplay) {
                        int diff = abs((int)((int)conn->modes[m].hdisplay - (int)drm->width)) + 
                                   abs((int)((int)conn->modes[m].vdisplay - (int)drm->height));
                        if (diff < best_mode_diff) {
                            best_mode_diff = diff;
                            best_mode = m;
                        }
                    }
                }
                
                // If we found a suitable landscape mode
                if (best_mode >= 0) {
                    drm->mode = conn->modes[best_mode];
                    break;
                }
                
                // If no landscape mode found, we'll have to use software rotation
                // with the default mode
                drm->mode = conn->modes[0];
                break;
            }
        }
        drmModeFreeConnector(conn);
        conn = NULL;
    }
    
    // If DSI-1 not found, try any connected connector
    if (!conn) {
        for (int i = 0; i < res->count_connectors; i++) {
            conn = drmModeGetConnector(drm->fd, res->connectors[i]);
            if (conn->connection == DRM_MODE_CONNECTED) {
                drm->connector_id = conn->connector_id;
                drm->mode = conn->modes[0]; // Use the first mode
                break;
            }
            drmModeFreeConnector(conn);
            conn = NULL;
        }
    }
    
    if (!conn) {
        fprintf(stderr, "No connected connector found\n");
        drmModeFreeResources(res);
        close(drm->fd);
        free(drm);
        return NULL;
    }
    
    // Print the selected mode information
    printf("Selected display mode: %dx%d%c%d\n", 
           drm->mode.hdisplay, drm->mode.vdisplay,
           (drm->mode.flags & DRM_MODE_FLAG_INTERLACE) ? 'i' : 'p',
           drm->mode.vrefresh);
           
    // Store actual dimensions from the selected mode
    drm->width = drm->mode.hdisplay;
    drm->height = drm->mode.vdisplay;
    
    // Find encoder
    drmModeEncoder *enc = NULL;
    for (int i = 0; i < res->count_encoders; i++) {
        enc = drmModeGetEncoder(drm->fd, res->encoders[i]);
        if (enc->encoder_id == conn->encoder_id) {
            drm->crtc_id = enc->crtc_id;
            break;
        }
        drmModeFreeEncoder(enc);
        enc = NULL;
    }
    
    // If we didn't find the encoder from the connector, find any compatible one
    if (!enc) {
        for (int i = 0; i < res->count_encoders; i++) {
            enc = drmModeGetEncoder(drm->fd, res->encoders[i]);
            // Check if this encoder works with our connector
            for (int j = 0; j < conn->count_encoders; j++) {
                if (conn->encoders[j] == enc->encoder_id) {
                    // Find an available CRTC for this encoder
                    for (int k = 0; k < res->count_crtcs; k++) {
                        if (enc->possible_crtcs & (1 << k)) {
                            drm->crtc_id = res->crtcs[k];
                            break;
                        }
                    }
                    if (drm->crtc_id) break;
                }
            }
            if (drm->crtc_id) break;
            drmModeFreeEncoder(enc);
            enc = NULL;
        }
    }
    
    drmModeFreeConnector(conn);
    
    if (!drm->crtc_id) {
        fprintf(stderr, "No suitable encoder/CRTC found\n");
        drmModeFreeResources(res);
        close(drm->fd);
        free(drm);
        return NULL;
    }
    
    if (enc) drmModeFreeEncoder(enc);
    
    // Find plane
    drmModePlaneRes *plane_res = drmModeGetPlaneResources(drm->fd);
    if (!plane_res) {
        perror("Failed to get plane resources");
        drmModeFreeResources(res);
        close(drm->fd);
        free(drm);
        return NULL;
    }
    
    for (uint32_t i = 0; i < plane_res->count_planes; i++) {
        drmModePlane *plane = drmModeGetPlane(drm->fd, plane_res->planes[i]);
        if (plane->crtc_id == drm->crtc_id || plane->crtc_id == 0) {
            // Check if the plane supports XR24 format (XRGB8888)
            for (uint32_t j = 0; j < plane->count_formats; j++) {
                if (plane->formats[j] == DRM_FORMAT_XRGB8888) {
                    drm->plane_id = plane->plane_id;
                    drmModeFreePlane(plane);
                    break;
                }
            }
            if (drm->plane_id != 0) break;
        }
        drmModeFreePlane(plane);
    }
    
    drmModeFreePlaneResources(plane_res);
    drmModeFreeResources(res);
    printf("drm->plane_id = %d\n", drm->plane_id);
    if (drm->plane_id == 0) {
        fprintf(stderr, "No suitable plane found\n");
        close(drm->fd);
        free(drm);
        return NULL;
    }
    
    // Create a dumb buffer for the actual display dimensions
    memset(&drm->create_dumb, 0, sizeof(drm->create_dumb));
    drm->create_dumb.width = drm->width;
    drm->create_dumb.height = drm->height;
    drm->create_dumb.bpp = 32; // XRGB8888
    
    if (drmIoctl(drm->fd, DRM_IOCTL_MODE_CREATE_DUMB, &drm->create_dumb) < 0) {
        perror("Failed to create dumb buffer");
        close(drm->fd);
        free(drm);
        return NULL;
    }
    
    // Create framebuffer
    uint32_t handles[4] = {drm->create_dumb.handle};
    uint32_t pitches[4] = {drm->create_dumb.pitch};
    uint32_t offsets[4] = {0};
    
    if (drmModeAddFB2(drm->fd, drm->width, drm->height, DRM_FORMAT_XRGB8888,
                      handles, pitches, offsets, &drm->fb_id, 0) < 0) {
        perror("Failed to create framebuffer");
        drmIoctl(drm->fd, DRM_IOCTL_MODE_DESTROY_DUMB, &drm->create_dumb);
        close(drm->fd);
        free(drm);
        return NULL;
    }
    
    int dma_buf_fd = -1;
    if (drmPrimeHandleToFD(drm->fd, drm->create_dumb.handle, 0, &dma_buf_fd) < 0) {
        perror("Failed to export DRM buffer to DMA-BUF");
        fprintf(stderr, "DMA-BUF export failed, falling back to normal mode\n");
    } else {
        printf("Successfully exported DRM buffer to DMA-BUF fd: %d\n", dma_buf_fd);
        drm->dma_buf_fd = dma_buf_fd;
    }
    // Map buffer
    memset(&drm->map_dumb, 0, sizeof(drm->map_dumb));
    drm->map_dumb.handle = drm->create_dumb.handle;
    
    if (drmIoctl(drm->fd, DRM_IOCTL_MODE_MAP_DUMB, &drm->map_dumb) < 0) {
        perror("Failed to map dumb buffer");
        drmModeRmFB(drm->fd, drm->fb_id);
        drmIoctl(drm->fd, DRM_IOCTL_MODE_DESTROY_DUMB, &drm->create_dumb);
        close(drm->fd);
        free(drm);
        return NULL;
    }
    
    drm->map_addr = (uint8_t *)mmap(0, drm->create_dumb.size, PROT_READ | PROT_WRITE, MAP_SHARED,
                                    drm->fd, drm->map_dumb.offset);
    if (drm->map_addr == MAP_FAILED) {
        perror("Failed to mmap buffer");
        drmModeRmFB(drm->fd, drm->fb_id);
        drmIoctl(drm->fd, DRM_IOCTL_MODE_DESTROY_DUMB, &drm->create_dumb);
        close(drm->fd);
        free(drm);
        return NULL;
    }
    printf("drm->create_dumb.size = %llu\n", drm->create_dumb.size);
    // Clear the buffer to black
    memset(drm->map_addr, 0, drm->create_dumb.size);
    
    // Set CRTC
    if (drmModeSetCrtc(drm->fd, drm->crtc_id, drm->fb_id, 0, 0,
                       &drm->connector_id, 1, &drm->mode) < 0) {
        perror("Failed to set CRTC");
        munmap(drm->map_addr, drm->create_dumb.size);
        drmModeRmFB(drm->fd, drm->fb_id);
        drmIoctl(drm->fd, DRM_IOCTL_MODE_DESTROY_DUMB, &drm->create_dumb);
        close(drm->fd);
        free(drm);
        return NULL;
    }


    printf("DRM initialized with %dx%d display buffer\n", drm->width, drm->height);
    return drm;
}

void destroy_drm(My_drm_context_t *drm) {
    if (!drm) return;
    
        // 关闭DMA-BUF fd
    if (drm->dma_buf_fd >= 0) {
        close(drm->dma_buf_fd);
    }
    munmap(drm->map_addr, drm->create_dumb.size);
    drmModeRmFB(drm->fd, drm->fb_id);
    
    struct drm_mode_destroy_dumb destroy_dumb = {0};
    destroy_dumb.handle = drm->create_dumb.handle;
    drmIoctl(drm->fd, DRM_IOCTL_MODE_DESTROY_DUMB, &destroy_dumb);
    
    close(drm->fd);
    free(drm);
}
