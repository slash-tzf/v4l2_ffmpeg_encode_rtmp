cmake_minimum_required(VERSION 3.10)

project(v4l2_displayer_test)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#设置可执行程序输出到bin文件夹

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
# 设置交叉编译器路径
set(CMAKE_C_COMPILER /opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-gcc)  # 替换为您的 C 编译器路径
set(CMAKE_CXX_COMPILER /opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++)  # 替换为您的 C++ 编译器路径

# 设置目标系统的根目录（sysroot），用于查找目标系统的库和头文件
set(CMAKE_SYSROOT /home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/)  # sysroot 路径

# 设置查找程序、库和头文件的路径
set(CMAKE_FIND_ROOT_PATH /home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/usr/lib)  # sysroot 路径

set(SYSROOT /home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/usr/include)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${SYSROOT}/drm)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)


set(FFMPEG_INCLUDE_DIRS 
    /home/alientek/ffmpeg_armbuild/include
)
include_directories(${FFMPEG_INCLUDE_DIRS})

set(FFMPEG_LIBRARIES 
    /home/alientek/ffmpeg_armbuild/lib/libavcodec.so
    /home/alientek/ffmpeg_armbuild/lib/libavformat.so
    /home/alientek/ffmpeg_armbuild/lib/libavutil.so
    /home/alientek/ffmpeg_armbuild/lib/libswscale.so
    /home/alientek/ffmpeg_armbuild/lib/libswresample.so
)


add_executable(v4l2_displayer_test ${SRC_LIST})


target_link_libraries(v4l2_displayer_test rga ${FFMPEG_LIBRARIES} drm asound)