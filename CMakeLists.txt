cmake_minimum_required(VERSION 3.10)

project(v4l2_displayer_test)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# 添加O2优化
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(CMAKE_C_COMPILER /opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-gcc)  # 替换为您的 C 编译器路径
set(CMAKE_CXX_COMPILER /opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++)  # 替换为您的 C++ 编译器路径

set(CMAKE_SYSROOT /home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/)  # sysroot 路径

set(CMAKE_FIND_ROOT_PATH /home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib)  #lib路径

set(SYSROOT /home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/usr/include) #include路径

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SYSROOT}/drm)
include_directories(${SYSROOT}/rga)
include_directories(${SYSROOT}/rknn)
include_directories(${SYSROOT}/opencv4) 

set(RKNN_SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/src/rknn_inference.cc")

file(GLOB SRC_LIST 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc
)

set(FFMPEG_INCLUDE_DIRS 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ffmpeg/include
)
include_directories(${FFMPEG_INCLUDE_DIRS})

set(FFMPEG_LIBRARIES 
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ffmpeg/lib/libavcodec.so
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ffmpeg/lib/libavformat.so
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ffmpeg/lib/libavutil.so
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ffmpeg/lib/libswscale.so
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/ffmpeg/lib/libswresample.so
)


add_executable(v4l2_displayer_test ${SRC_LIST})


target_link_libraries(v4l2_displayer_test rga ${FFMPEG_LIBRARIES} drm rknn_api opencv_core opencv_imgproc opencv_highgui)
