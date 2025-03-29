#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include "image_converter.h"
#include "buffer_manager.h"
#include "camera.h"
#include <rga/im2d.h>
#include "drm_disp.h"
#include "libavutil/pixfmt.h"
#include "rga.h"
#include "rknn_yolov5.h"
#include "postprocess.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/avassert.h>
#include <libavutil/opt.h>
}

// 16字节对齐函数
static inline size_t align_to_16(size_t size) {
    return (size + 15) & ~15;
}

// Initialize buffer manager
buffer_manager_t* init_buffer_manager(int buffer_count, int width, int height) {
    buffer_manager_t *mgr = (buffer_manager_t*)malloc(sizeof(buffer_manager_t));
    if (!mgr) {
        perror("Failed to allocate buffer manager");
        return NULL;
    }

    mgr->buffers = (frame_buffer_t*)malloc(buffer_count * sizeof(frame_buffer_t));
    if (!mgr->buffers) {
        perror("Failed to allocate buffer array");
        free(mgr);
        return NULL;
    }
    // 计算NV12和BGRA格式所需的对齐内存大小
    size_t nv12_size = align_to_16( height) * width * 3 / 2; // NV12格式
    size_t bgra_size = align_to_16( height) * width * 4; // bgra格式
    size_t RGB_size = align_to_16( height) * width * 3; // RGB格式
    for (int i = 0; i < buffer_count; i++) {
        // 分配足够大的内存以处理BGRA格式（更大的格式）
        mgr->buffers[i].data = (char*)malloc(bgra_size);
        if (!mgr->buffers[i].data) {
            perror("Failed to allocate buffer");
            // Clean up already allocated memory
            for (int j = 0; j < i; j++) {
                free(mgr->buffers[j].data);
            }
            free(mgr->buffers);
            free(mgr);
            return NULL;
        }
        mgr->buffers[i].size = bgra_size;  // 初始设置为最大可能的大小
        mgr->buffers[i].frame_index = -1;
        mgr->buffers[i].timestamp = 0;
    }

    mgr->buffer_count = buffer_count;
    mgr->capture_index = 0;
    mgr->process_index = 0;
    mgr->display_index = 0;
    mgr->encode_index = 0;
    mgr->running = 1;
    mgr->width = width;
    mgr->height = height;
    mgr->nv12_size = nv12_size;
    mgr->encode_buffer = (char*)malloc(nv12_size);
    mgr->RGB_buffer = (char*)malloc(RGB_size);

    mgr->bgra_size = bgra_size;
    mgr->RGB_size = RGB_size;
    mgr->stride = width * 4;
    mgr->detect_result = {0};

    // Initialize synchronization primitives
    sem_init(&mgr->empty_sem, 0, buffer_count);
    sem_init(&mgr->filled_sem, 0, 0);
    sem_init(&mgr->encode_sem, 0, 0);
    sem_init(&mgr->display_sem, 0, 0);
    pthread_mutex_init(&mgr->mutex, NULL);
    return mgr;
}

// Destroy buffer manager and free resources
void destroy_buffer_manager(buffer_manager_t *mgr) {
    if (!mgr) return;

    // Free individual frame buffers
    if (mgr->buffers) {
        for (int i = 0; i < mgr->buffer_count; i++) {
            if (mgr->buffers[i].data) {
                free(mgr->buffers[i].data);
            }
        }
        free(mgr->buffers);
    }

    // Free other buffers
    if (mgr->encode_buffer) {
        free(mgr->encode_buffer);
    }
    if (mgr->RGB_buffer) {
        free(mgr->RGB_buffer);
    }

    // Destroy synchronization primitives
    sem_destroy(&mgr->encode_sem);
    sem_destroy(&mgr->empty_sem);
    sem_destroy(&mgr->filled_sem);
    sem_destroy(&mgr->display_sem);
    pthread_mutex_destroy(&mgr->mutex);

    // Finally free the manager itself
    free(mgr);
}

// Capture thread function
void* video_capture_thread_func(void *arg) {
    thread_params_t *params = (thread_params_t*)arg;
    struct v4l2_dev *camdev = params->camdev;
    buffer_manager_t *mgr = params->buffer_mgr;
    char name[40] = {0};
    printf("Capture thread started\n");
    
    while (mgr->running) {
        // Wait for empty buffer
        sem_wait(&mgr->empty_sem);
        
        if (!mgr->running) break;
        
        // Get a frame
        get_frame(camdev, 0);
        
        pthread_mutex_lock(&mgr->mutex);
        int idx = mgr->capture_index;
        mgr->capture_index = (mgr->capture_index + 1) % mgr->buffer_count;
        pthread_mutex_unlock(&mgr->mutex);

        
        // Copy frame data to buffer (NV12 format)
        memcpy(mgr->buffers[idx].data, camdev->out_data, camdev->data_len);
        mgr->buffers[idx].size = camdev->data_len;  // 实际大小（NV12）
        mgr->buffers[idx].frame_index = idx;
        mgr->buffers[idx].timestamp = camdev->timestamp;

        sem_post(&mgr->filled_sem);
        
    }
    
    printf("Capture thread exiting\n");
    return NULL;
}



// Process thread function
void* Inference_thread_func(void *arg) {
    thread_params_t *params = (thread_params_t*)arg;
    buffer_manager_t *mgr = params->buffer_mgr;
    int width = mgr->width;
    int height = mgr->height;
    int ret;
    printf("Process thread started\n");
    
    // Initialize RKNN model
    RknnYolov5 rknn;
    ret = rknn.Init("./model/yolov5s-640-640.rknn");
    if (ret < 0) {
        fprintf(stderr, "Failed to initialize RKNN model\n");
        return NULL;
    }
    
    while (mgr->running) {
        // Wait for filled buffer
        sem_wait(&mgr->filled_sem);
        
        if (!mgr->running) break;
        
        pthread_mutex_lock(&mgr->mutex);
        int idx = mgr->process_index;
        mgr->process_index = (mgr->process_index + 1) % mgr->buffer_count;
        pthread_mutex_unlock(&mgr->mutex);
        
        ret = rknn.Inference((unsigned char*)mgr->buffers[idx].data, width, height, &(mgr->detect_result));
        if (ret < 0) {
            printf("Inference failed!\n");
        } else {
            //printf("Detect Result Count: %d\n", mgr->detect_result.count);
        }
        sem_post(&mgr->display_sem);
    }

    printf("Inference thread exiting\n");
    return NULL;
}

void* display_thread_func(void *arg){
    thread_params_t *params = (thread_params_t*)arg;
    buffer_manager_t *mgr = params->buffer_mgr;
    int width = mgr->width;
    int height = mgr->height;
    int ret;

        // Initialize DRM
    My_drm_context_t *drm = init_drm(width, height);
    if (!drm) {
        fprintf(stderr, "Failed to initialize DRM, exiting display thread\n");
        return NULL;
    }

    // Print buffer dimensions
    printf("Input buffer: %dx%d, Display buffer: %dx%d\n", 
        width, height, drm->width, drm->height);

        // Variables for FPS calculation
    int frame_count = 0;
    float fps = 0.0f;
    struct timeval start_time, current_time;
    gettimeofday(&start_time, NULL);

    while (mgr->running) {

        sem_wait(&mgr->display_sem);
        
        if (!mgr->running) break;

        pthread_mutex_lock(&mgr->mutex);
        int idx = mgr->display_index;
        mgr->display_index = (mgr->display_index + 1) % mgr->buffer_count;
        pthread_mutex_unlock(&mgr->mutex);

        ret = convert_nv12_to_RGB(mgr->buffers[idx].data, mgr->RGB_buffer, width, height);
        if (ret != IM_STATUS_SUCCESS) {
            printf("Error converting NV12 to RGB: %s\n", imStrError((IM_STATUS)ret));
            continue;
        }
    
        // Calculate FPS
        frame_count++;
        gettimeofday(&current_time, NULL);
        float time_elapsed = ((current_time.tv_sec - start_time.tv_sec) * 1000000.0f +
                            (current_time.tv_usec - start_time.tv_usec)) / 1000000.0f;
        if (time_elapsed >= 1.0f) {
            fps = frame_count / time_elapsed;
            frame_count = 0;
            start_time = current_time;
            printf("FPS = %.1f \n",fps);
        }
    
        // Create OpenCV Mat for drawing
        cv::Mat rgb_frame(height, width, CV_8UC3, mgr->RGB_buffer);
    
        // Draw FPS
        char fps_text[32];
        snprintf(fps_text, sizeof(fps_text), "FPS: %.1f", fps);
        cv::putText(rgb_frame, fps_text, cv::Point(30, 50), cv::FONT_HERSHEY_SIMPLEX, 
                    1.5, cv::Scalar(0, 255, 0), 2);
    
        // Draw detection boxes
        for (int i = 0; i < mgr->detect_result.count; i++) {
            detect_result_t* det = &(mgr->detect_result.results[i]);
            int x1 = det->box.left;
            int y1 = det->box.top;
            int x2 = det->box.right;
            int y2 = det->box.bottom;
    
            // Draw rectangle
            cv::rectangle(rgb_frame, cv::Point(x1, y1), cv::Point(x2, y2), cv::Scalar(255, 0, 0), 3);
    
            // Format text with class name and confidence
            char text[128];
            snprintf(text, sizeof(text), "%s %.2f", det->name, det->prop);
    
            // Draw text
            cv::putText(rgb_frame, text, cv::Point(x1, y1 - 12), cv::FONT_HERSHEY_SIMPLEX, 1, cv::Scalar(255, 0, 0),2);
            
            //printf("%s @ (%d, %d, %d, %d) %.3f\n", det->name, x1, y1, x2, y2, det->prop);
        }
    
        // Convert RGB to BGRA
        ret = convert_RGB_to_BGRA_dma_buf(mgr->RGB_buffer, drm, width, height);
        if (ret != IM_STATUS_SUCCESS) {
            printf("Error converting RGB to BGRA: %s\n", imStrError((IM_STATUS)ret));
            continue;
        }
    
        if (drmModePageFlip(drm->fd, drm->crtc_id, drm->fb_id, 
            DRM_MODE_PAGE_FLIP_EVENT, NULL) < 0) {
            perror("Failed to page flip");
        } else {
            // Wait for page flip to complete
            drmEventContext evctx = {0};
            evctx.version = DRM_EVENT_CONTEXT_VERSION;
            evctx.page_flip_handler = NULL;
    
            fd_set fds;
            FD_ZERO(&fds);
            FD_SET(drm->fd, &fds);
    
            struct timeval timeout = {0, 100000}; // 100ms timeout
    
            select(drm->fd + 1, &fds, NULL, NULL, &timeout);
            if (FD_ISSET(drm->fd, &fds))
                drmHandleEvent(drm->fd, &evctx);
        }
    
        sem_post(&mgr->empty_sem);
        sem_post(&mgr->encode_sem);
    }

        // Cleanup DRM
    if (drm) {
        // Wait for any pending operations to complete
        drmEventContext evctx = {0};
        evctx.version = DRM_EVENT_CONTEXT_VERSION;
        evctx.page_flip_handler = NULL;
        
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(drm->fd, &fds);
        
        struct timeval timeout = {0, 100000}; // 100ms timeout
        select(drm->fd + 1, &fds, NULL, NULL, &timeout);
        if (FD_ISSET(drm->fd, &fds)) {
            drmHandleEvent(drm->fd, &evctx);
        }
        
        destroy_drm(drm);
        drm = NULL;
    }

    printf("Display thread exiting\n");
    return NULL;
}

void* encode_thread_func(void *arg) {
    thread_params_t *params = (thread_params_t*)arg;
    buffer_manager_t *mgr = params->buffer_mgr;
    int width = mgr->width;
    int height = mgr->height;
    
    printf("Encode thread started\n");
    
    // 初始化FFmpeg网络功能
    avformat_network_init();
    
    // RTMP流地址
    const char *rtmp_url = "rtmp://127.0.0.1:1935/live/test";
    
    // 设置输出格式上下文
    AVFormatContext *out_ctx = NULL;
    avformat_alloc_output_context2(&out_ctx, NULL, "flv", rtmp_url); // 使用FLV格式进行RTMP流
    if (!out_ctx) {
        fprintf(stderr, "Could not create output context\n");
        return NULL;
    }
    
    // 查找H.264编码器
    const AVCodec *codec = avcodec_find_encoder_by_name("h264_rkmpp");
    if (!codec) {
        fprintf(stderr, "Could not find H.264 encoder\n");
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    // 创建编码器上下文
    AVCodecContext *codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        fprintf(stderr, "Could not allocate encoding context\n");
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    // 设置编码参数
    codec_ctx->width = width;
    codec_ctx->height = height;
    codec_ctx->time_base = (AVRational){1, 15}; // 30fps
    codec_ctx->framerate = (AVRational){15, 1};
    codec_ctx->pix_fmt = AV_PIX_FMT_YUV420P; // 使用YUV420P格式编码
    codec_ctx->bit_rate = 10000000; // 5 Mbps，可以根据网络情况调整
    
    // 对于RTMP流，设置一些特定的参数
    codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER; // RTMP需要全局头
    codec_ctx->max_b_frames = 0; // RTMP流不使用B帧，更好的实时性
    codec_ctx->gop_size = 10; // 每秒一个关键帧
    
    // 打开编码器
    if (avcodec_open2(codec_ctx, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        avcodec_free_context(&codec_ctx);
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    // 创建视频流
    AVStream *stream = avformat_new_stream(out_ctx, NULL);
    if (!stream) {
        fprintf(stderr, "Could not create stream\n");
        avcodec_free_context(&codec_ctx);
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    stream->id = out_ctx->nb_streams - 1;
    avcodec_parameters_from_context(stream->codecpar, codec_ctx);
    
    // 打开RTMP输出连接
    // 注意：RTMP总是需要AVFMT_NOFILE标志，因此不检查该标志
    if (avio_open(&out_ctx->pb, rtmp_url, AVIO_FLAG_WRITE) < 0) {
        fprintf(stderr, "Could not open output URL '%s'\n", rtmp_url);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    // 写入流头信息
    if (avformat_write_header(out_ctx, NULL) < 0) {
        fprintf(stderr, "Error writing header\n");
        avio_closep(&out_ctx->pb);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    // 创建帧
    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate frame\n");
        avio_closep(&out_ctx->pb);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    frame->format = codec_ctx->pix_fmt;
    frame->width = codec_ctx->width;
    frame->height = codec_ctx->height;
    
    // 分配帧缓冲区
    if (av_frame_get_buffer(frame, 0) < 0) {
        fprintf(stderr, "Could not allocate frame data\n");
        av_frame_free(&frame);
        avio_closep(&out_ctx->pb);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    // 创建数据包
    AVPacket *pkt = av_packet_alloc();
    if (!pkt) {
        fprintf(stderr, "Could not allocate packet\n");
        av_frame_free(&frame);
        avio_closep(&out_ctx->pb);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    // 为格式转换创建上下文
    struct SwsContext *sws_ctx = sws_getContext(
        width, height, AV_PIX_FMT_NV12,  // 源格式
        width, height, AV_PIX_FMT_YUV420P,  // 目标格式 
        SWS_BILINEAR, NULL, NULL, NULL);
    
    if (!sws_ctx) {
        fprintf(stderr, "Could not initialize the conversion context\n");
        av_packet_free(&pkt);
        av_frame_free(&frame);
        avio_closep(&out_ctx->pb);
        avcodec_free_context(&codec_ctx);
        avformat_free_context(out_ctx);
        return NULL;
    }
    
    int frame_count = 0;
    int64_t pts = 0;
    struct timeval start_time, end_time;
    // 主循环: 获取帧，转换，编码，推流
    while (mgr->running) {
        // 等待新帧可用
        sem_wait(&mgr->encode_sem);
        
        if (!mgr->running) break;
        
        // 确保帧数据可写
        if (av_frame_make_writable(frame) < 0) {
            fprintf(stderr, "Could not make frame writable\n");
            continue;
        }

        //RGB转NV12
        convert_color(mgr->RGB_buffer, mgr->encode_buffer, width, height, RK_FORMAT_RGB_888, RK_FORMAT_YCbCr_420_SP);

                // 准备源数据指针
        const uint8_t *src_data[2] = {
            (uint8_t*)mgr->encode_buffer,                         // Y平面
            (uint8_t*)mgr->encode_buffer + (width * height)       // UV平面
        };
        int src_linesize[2] = { width, width };  // NV12格式的跨步

        sws_scale(sws_ctx, src_data, src_linesize, 0, height,
                  frame->data, frame->linesize);
        
        // 设置帧的PTS
        frame->pts = pts++;
        
        // 将帧发送给编码器
        int ret = avcodec_send_frame(codec_ctx, frame);
        if (ret < 0) {
            fprintf(stderr, "Error sending a frame for encoding\n");
            continue;
        }
        
        // 从编码器接收数据包
        while (ret >= 0) {
            ret = avcodec_receive_packet(codec_ctx, pkt);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
                break;
            } else if (ret < 0) {
                fprintf(stderr, "Error during encoding\n");
                break;
            }
            
            // 转换时间基准
            av_packet_rescale_ts(pkt, codec_ctx->time_base, stream->time_base);
            pkt->stream_index = stream->index;
            
            // 写入数据包到RTMP流
            ret = av_interleaved_write_frame(out_ctx, pkt);
            if (ret < 0) {
                fprintf(stderr, "Error writing packet\n");
                break;
            }
            
            frame_count++;
            if (frame_count % 100 == 0) {
                printf("Streamed %d frames\n", frame_count);
            }
        }
    }
    
    // 写入流尾
    av_write_trailer(out_ctx);
    
    // 清理资源
    sws_freeContext(sws_ctx);
    av_packet_free(&pkt);
    av_frame_free(&frame);
    avcodec_free_context(&codec_ctx);
    avio_closep(&out_ctx->pb);
    avformat_free_context(out_ctx);
    
    printf("Encode thread exiting, streamed %d frames to %s\n", frame_count, rtmp_url);
    return NULL;
}


int main_multithreaded(struct v4l2_dev *camdev, int width, int height) {
    // 初始化缓冲区管理器，使用2个缓冲区，传入宽高参数
    buffer_manager_t *buffer_mgr = init_buffer_manager(2, width, height);
    if (!buffer_mgr) {
        fprintf(stderr, "Failed to initialize buffer manager\n");
        return -1;
    }
    
    // 设置线程参数
    thread_params_t params = {
        .camdev = camdev, 
        .buffer_mgr = buffer_mgr,
        .width = width,
        .height = height,
    };
    
    // 创建线程
    pthread_t video_capture_thread, Inference_thread, encode_thread, display_thread;
    pthread_create(&video_capture_thread, NULL, video_capture_thread_func, &params);
    pthread_create(&Inference_thread, NULL, Inference_thread_func, &params);
    pthread_create(&display_thread, NULL, display_thread_func, &params);
    pthread_create(&encode_thread, NULL, encode_thread_func, &params);
    
    // 等待用户输入来退出
    printf("Press Enter to stop...\n");
    getchar();

    // 停止所有线程
    buffer_mgr->running = 0;
    sem_post(&buffer_mgr->filled_sem);
    sem_post(&buffer_mgr->encode_sem);
    sem_post(&buffer_mgr->display_sem);
    sem_post(&buffer_mgr->empty_sem);
    pthread_join(video_capture_thread, NULL);
    pthread_join(Inference_thread, NULL);
    pthread_join(display_thread, NULL);
    pthread_join(encode_thread, NULL);
    // 清理缓冲区管理器
    destroy_buffer_manager(buffer_mgr);

    printf("Main thread exiting\n");
    return 0;
}
