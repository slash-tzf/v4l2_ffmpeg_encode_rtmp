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
    .data_len = 1920 * 1080 * 3 / 2,
    .out_data = NULL,
};

int main()
{
    struct v4l2_dev *camdev = &im335;
    // 初始化摄像头
    camera_init(camdev);

    // 启动多线程
    main_multithreaded(camdev, camdev->width, camdev->height);

    // 清理资源
    camera_deinit(camdev);

    return 0;
}
