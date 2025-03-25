#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <math.h>
#include <fcntl.h>

#include <unistd.h>
#include <linux/stddef.h>
#include "camera.h"
#include "framebuffer.h"
#include "buffer_manager.h"
#include <rga/im2d.h>

struct v4l2_dev im355 = {
    .fd = -1,
    .sub_fd = -1,
    .path = "/dev/video0",
    .name = "MX335",
    .subdev_path = "/dev/v4l-subdev0",
    .out_type = "nv12",
    .buf_type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE,
    .format = V4L2_PIX_FMT_NV12,
    .width = 1920,
    .height = 1080,
    .req_count = 4,
    .memory_type = V4L2_MEMORY_MMAP,
    .buffers = NULL,
    .timestamp = 0,
    .data_len = 0,
    .out_data = NULL,
};

int main()
{
    struct v4l2_dev *camdev = &im355;
unsigned int fps = 0;
    unsigned int mode = 4;
    char name[40] = {0};
lcd_color color_buff;

    open_device(camdev); // 1 打开摄像头设备
    get_capabilities(camdev);
    set_fmt(camdev);                 // 2 设置出图格式
    require_buf(camdev);          // 3 申请缓冲区
    alloc_buf(camdev);            // 4 内存映射
    queue_buf(camdev);            // 5 将缓存帧加入队列
    set_fps(camdev, fps);

    stream_on(camdev);            // 6 开启视频流
    
    printf("sizeof RGBA = %lu\n", sizeof(color_buff));
    printf("sizeof screen = %lu\n", screen_size);

    main_multithreaded(camdev, camdev->width, camdev->height); // 7 启动多线程

    stream_off(camdev);           // 8 关闭视频流

    printf("ALSA device closed\n");

    close_device(camdev);        // 9 关闭设备

    return 0;

}
