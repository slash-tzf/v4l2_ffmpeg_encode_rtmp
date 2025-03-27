#include "camera.h"
#include "buffer_manager.h"

struct v4l2_dev im335 = {
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
    struct v4l2_dev *camdev = &im335;

    camera_init(camdev); // 初始化摄像头

    main_multithreaded(camdev, camdev->width, camdev->height); // 启动多线程

    camera_deinit(camdev); // 关闭摄像头

    return 0;

}
