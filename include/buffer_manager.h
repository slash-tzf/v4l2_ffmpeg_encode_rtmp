#ifndef BUFFER_MANAGER_H
#define BUFFER_MANAGER_H

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdint.h>
#include "rknn_yolov5.h"
// Forward declaration
struct v4l2_dev;

// Define a frame buffer structure
typedef struct {
    char *data;
    size_t size;
    int frame_index;
    unsigned long timestamp;
} frame_buffer_t;


// 添加音频缓冲区结构
typedef struct {
    char *data;          // 音频数据
    size_t size;         // 缓冲区大小
    size_t data_size;    // 实际数据大小
    int64_t timestamp;   // 时间戳
} audio_buffer_t;

// Define a buffer manager structure
typedef struct {
    frame_buffer_t *buffers;
    int buffer_count;
    int capture_index;
    int process_index;
    int display_index;
    sem_t empty_sem;
    sem_t filled_sem;
    sem_t display_sem;
    pthread_mutex_t mutex;
    int running;
    int width;
    int height;
    int stride;
    size_t nv12_size;
    size_t bgra_size;
    size_t RGB_size;
    char* RGB_buffer;
    sem_t encode_sem;
    int encode_index;
    char *encode_buffer;
    detect_result_group_t detect_result;
} buffer_manager_t;


typedef struct {
    struct v4l2_dev *camdev;
    buffer_manager_t *buffer_mgr;
    int width;
    int height;
    int screen_size;
} thread_params_t;


// Buffer manager functions
buffer_manager_t* init_buffer_manager(int buffer_count,  int width, int height);
void destroy_buffer_manager(buffer_manager_t *mgr);

// Thread functions
void* video_capture_thread_func(void *arg);
void* Inference_thread_func(void *arg);
void* encode_thread_func(void *arg);
void* audio_capture_thread_func(void *arg);
// Main multithreaded processing function
int main_multithreaded(struct v4l2_dev *camdev, int width, int height);

#endif /* BUFFER_MANAGER_H */
