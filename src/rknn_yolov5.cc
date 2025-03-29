#include "rknn_yolov5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include "rga/im2d.h"
#include "rga/rga.h"

RknnYolov5::RknnYolov5() : ctx(0), input_buffer(nullptr) {
    for (int i = 0; i < 3; i++) {
        output_buffer[i] = nullptr;
    }
}

RknnYolov5::~RknnYolov5() {
    Release();
}

static void dump_tensor_attr(rknn_tensor_attr* attr) {
    std::string shape_str = attr->n_dims < 1 ? "" : std::to_string(attr->dims[0]);
    for (int i = 1; i < attr->n_dims; ++i) {
        shape_str += ", " + std::to_string(attr->dims[i]);
    }
    
    printf("  index=%d, name=%s, n_dims=%d, dims=[%s], n_elems=%d, size=%d, fmt=%s, type=%s, qnt_type=%s, "
           "zp=%d, scale=%f\n",
           attr->index, attr->name, attr->n_dims, shape_str.c_str(), attr->n_elems, attr->size,
           get_format_string(attr->fmt), get_type_string(attr->type),
           get_qnt_type_string(attr->qnt_type), attr->zp, attr->scale);
}

int RknnYolov5::Init(const char* model_path, int width, int height) {
    printf("Loading model: %s\n", model_path);
    
    // Load RKNN Model
    FILE* fp = fopen(model_path, "rb");
    if (!fp) {
        printf("Failed to open model file: %s\n", model_path);
        return -1;
    }
    
    fseek(fp, 0, SEEK_END);
    size_t model_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    unsigned char* model = (unsigned char*)malloc(model_size);
    if (!model) {
        fclose(fp);
        return -1;
    }
    
    if (fread(model, 1, model_size, fp) != model_size) {
        free(model);
        fclose(fp);
        return -1;
    }
    fclose(fp);
    
    int ret = rknn_init(&ctx, model, model_size, 0, NULL);
    free(model);
    
    if (ret < 0) {
        printf("rknn_init fail! ret=%d\n", ret);
        return -1;
    }
    
    // Get SDK Version
    rknn_sdk_version version;
    ret = rknn_query(ctx, RKNN_QUERY_SDK_VERSION, &version, sizeof(rknn_sdk_version));
    if (ret < 0) {
        printf("rknn_query SDK version error ret=%d\n", ret);
        return -1;
    }
    printf("sdk version: %s driver version: %s\n", version.api_version, version.drv_version);
    
    // Get Model Info
    rknn_input_output_num io_num;
    ret = rknn_query(ctx, RKNN_QUERY_IN_OUT_NUM, &io_num, sizeof(io_num));
    if (ret != RKNN_SUCC) {
        printf("rknn_query io_num error ret=%d\n", ret);
        return -1;
    }
    printf("model input num: %d, output num: %d\n", io_num.n_input, io_num.n_output);
    
    // Get Input Tensors
    rknn_tensor_attr input_attrs[io_num.n_input];
    memset(input_attrs, 0, sizeof(input_attrs));
    for (int i = 0; i < io_num.n_input; i++) {
        input_attrs[i].index = i;
        ret = rknn_query(ctx, RKNN_QUERY_INPUT_ATTR, &(input_attrs[i]), sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC) {
            printf("rknn_query input_attrs error ret=%d\n", ret);
            return -1;
        }
        dump_tensor_attr(&(input_attrs[i]));
    }

    // Determine input format
    if (input_attrs[0].fmt == RKNN_TENSOR_NCHW) {
        printf("model is NCHW input fmt\n");
        channel = input_attrs[0].dims[1];
        model_height = input_attrs[0].dims[2];
        model_width = input_attrs[0].dims[3];
    } else {
        printf("model is NHWC input fmt\n");
        model_height = input_attrs[0].dims[1];
        model_width = input_attrs[0].dims[2];
        channel = input_attrs[0].dims[3];
    }
    printf("model input height=%d, width=%d, channel=%d\n", model_height, model_width, channel);
    
    // Get Output Tensor Info
    rknn_tensor_attr output_attrs[io_num.n_output];
    memset(output_attrs, 0, sizeof(output_attrs));
    for (int i = 0; i < io_num.n_output; i++) {
        output_attrs[i].index = i;
        ret = rknn_query(ctx, RKNN_QUERY_OUTPUT_ATTR, &(output_attrs[i]), sizeof(rknn_tensor_attr));
        if (ret != RKNN_SUCC) {
            printf("rknn_query fail! ret=%d\n", ret);
            return -1;
        }
        
        qnt_zps.push_back(output_attrs[i].zp);
        qnt_scales.push_back(output_attrs[i].scale);
        
        // Pre-allocate output buffers
        int buffer_size = output_attrs[i].size_with_stride;
        output_buffer[i] = (int8_t*)malloc(buffer_size);
        if (!output_buffer[i]) {
            printf("Malloc output buffer %d failed!\n", i);
            return -1;
        }
        memset(output_buffer[i], 0, buffer_size);
        
        outputs[i].want_float = 0;
        outputs[i].is_prealloc = 1;
        outputs[i].index = i;
        outputs[i].size = output_attrs[i].size_with_stride;
        outputs[i].buf = output_buffer[i];
    }
    
    // Pre-allocate input buffer
    input_buffer = (unsigned char*)malloc(model_width * model_height * channel);
    if (!input_buffer) {
        printf("Malloc input buffer failed!\n");
        return -1;
    }
    
    return 0;
}

int RknnYolov5::PreProcess(unsigned char* input_data, int img_width, int img_height) {
    rga_buffer_t src = {0};
    rga_buffer_t dst = {0};
    im_rect src_rect = {0, 0, img_width, img_height};
    im_rect dst_rect = {0, 0, model_width, model_height};

    src = wrapbuffer_virtualaddr((void*)input_data, img_width, img_height, RK_FORMAT_YCbCr_420_SP);

    dst = wrapbuffer_virtualaddr((void*)input_buffer, model_width, model_height, RK_FORMAT_RGB_888);
    int ret = imresize(src, dst);
    if (ret != IM_STATUS_SUCCESS) {
        printf("Pre-process failed: %s\n", imStrError((IM_STATUS)ret));
        return -1;
    }
    
    // Set input tensor
    inputs[0].index = 0;
    inputs[0].type = RKNN_TENSOR_UINT8;
    inputs[0].size = model_width * model_height * channel;
    inputs[0].fmt = RKNN_TENSOR_NHWC;
    inputs[0].buf = input_buffer;
    
    return 0;
}

int RknnYolov5::Inference(unsigned char* input_data, int img_width, int img_height, detect_result_group_t* detect_result) {

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);
    int ret = PreProcess(input_data, img_width, img_height);
    if (ret < 0) {
        return -1;
    }
    
    ret = rknn_inputs_set(ctx, 1, inputs);
    if (ret < 0) {
        printf("rknn_input_set fail! ret=%d\n", ret);
        return -1;
    }
    

    
    ret = rknn_run(ctx, NULL);
    if (ret < 0) {
        printf("rknn_run fail! ret=%d\n", ret);
        return -1;
    }
    
    ret = rknn_outputs_get(ctx, 3, outputs, NULL);
    if (ret < 0) {
        printf("rknn_outputs_get fail! ret=%d\n", ret);
        return -1;
    }


    
    // Post Process
    float scale_w = (float)model_width / img_width;
    float scale_h = (float)model_height / img_height;
    
    ret = post_process((int8_t*)outputs[0].buf, (int8_t*)outputs[1].buf, (int8_t*)outputs[2].buf,
                       model_height, model_width, BOX_THRESH, NMS_THRESH, scale_w, scale_h,
                       qnt_zps, qnt_scales, detect_result);
    
    gettimeofday(&end_time, NULL);
    float inference_time = ((end_time.tv_sec - start_time.tv_sec) * 1000000 + (end_time.tv_usec - start_time.tv_usec)) / 1000.0f;
    printf("Inference time: %.2f ms\n", inference_time);
    rknn_outputs_release(ctx, 3, outputs);
    
    return ret;
}

void RknnYolov5::Release() {
    if (ctx > 0) {
        rknn_destroy(ctx);
        ctx = 0;
    }
    
    if (input_buffer) {
        free(input_buffer);
        input_buffer = nullptr;
    }
    
    for (int i = 0; i < 3; i++) {
        if (output_buffer[i]) {
            free(output_buffer[i]);
            output_buffer[i] = nullptr;
        }
    }
}
