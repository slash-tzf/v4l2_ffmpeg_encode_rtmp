#ifndef RKNN_YOLOV5_H
#define RKNN_YOLOV5_H

#include <stdint.h>
#include <vector>
#include <string>
#include "rknn_api.h"
#include "postprocess.h"


class RknnYolov5 {
public:
    RknnYolov5();
    ~RknnYolov5();
    
    int Init(const char* model_path, int width = 640, int height = 640);
    int Inference(unsigned char* input_data, int img_width, int img_height, detect_result_group_t* detect_result);
    void Release();

private:
    int PreProcess(unsigned char* input_data, int img_width, int img_height);
    
    rknn_context ctx;
    int model_width;
    int model_height;
    int channel;
    int input_index;
    std::vector<int32_t> qnt_zps;
    std::vector<float> qnt_scales;
    
    // Input/output tensors
    rknn_input inputs[1];
    rknn_output outputs[3];
    
    // Pre-allocated buffers
    unsigned char* input_buffer;
    int8_t* output_buffer[3];
};

#endif // RKNN_YOLOV5_H
