# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/alientek/workspace/v4l2_displayer_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alientek/workspace/v4l2_displayer_test/build

# Include any dependencies generated for this target.
include CMakeFiles/v4l2_displayer_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/v4l2_displayer_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/v4l2_displayer_test.dir/flags.make

CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.o: CMakeFiles/v4l2_displayer_test.dir/flags.make
CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.o: ../src/buffer_manager.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.o"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.o -c /home/alientek/workspace/v4l2_displayer_test/src/buffer_manager.cc

CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.i"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alientek/workspace/v4l2_displayer_test/src/buffer_manager.cc > CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.i

CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.s"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alientek/workspace/v4l2_displayer_test/src/buffer_manager.cc -o CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.s

CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.o: CMakeFiles/v4l2_displayer_test.dir/flags.make
CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.o: ../src/camera.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.o"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.o -c /home/alientek/workspace/v4l2_displayer_test/src/camera.cc

CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.i"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alientek/workspace/v4l2_displayer_test/src/camera.cc > CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.i

CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.s"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alientek/workspace/v4l2_displayer_test/src/camera.cc -o CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.s

CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.o: CMakeFiles/v4l2_displayer_test.dir/flags.make
CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.o: ../src/drm_disp.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.o"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.o -c /home/alientek/workspace/v4l2_displayer_test/src/drm_disp.cc

CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.i"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alientek/workspace/v4l2_displayer_test/src/drm_disp.cc > CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.i

CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.s"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alientek/workspace/v4l2_displayer_test/src/drm_disp.cc -o CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.s

CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.o: CMakeFiles/v4l2_displayer_test.dir/flags.make
CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.o: ../src/framebuffer.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.o"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.o -c /home/alientek/workspace/v4l2_displayer_test/src/framebuffer.cc

CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.i"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alientek/workspace/v4l2_displayer_test/src/framebuffer.cc > CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.i

CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.s"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alientek/workspace/v4l2_displayer_test/src/framebuffer.cc -o CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.s

CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.o: CMakeFiles/v4l2_displayer_test.dir/flags.make
CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.o: ../src/image_converter.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.o"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.o -c /home/alientek/workspace/v4l2_displayer_test/src/image_converter.cc

CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.i"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alientek/workspace/v4l2_displayer_test/src/image_converter.cc > CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.i

CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.s"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alientek/workspace/v4l2_displayer_test/src/image_converter.cc -o CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.s

CMakeFiles/v4l2_displayer_test.dir/src/main.cc.o: CMakeFiles/v4l2_displayer_test.dir/flags.make
CMakeFiles/v4l2_displayer_test.dir/src/main.cc.o: ../src/main.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/v4l2_displayer_test.dir/src/main.cc.o"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/v4l2_displayer_test.dir/src/main.cc.o -c /home/alientek/workspace/v4l2_displayer_test/src/main.cc

CMakeFiles/v4l2_displayer_test.dir/src/main.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/v4l2_displayer_test.dir/src/main.cc.i"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alientek/workspace/v4l2_displayer_test/src/main.cc > CMakeFiles/v4l2_displayer_test.dir/src/main.cc.i

CMakeFiles/v4l2_displayer_test.dir/src/main.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/v4l2_displayer_test.dir/src/main.cc.s"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alientek/workspace/v4l2_displayer_test/src/main.cc -o CMakeFiles/v4l2_displayer_test.dir/src/main.cc.s

CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.o: CMakeFiles/v4l2_displayer_test.dir/flags.make
CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.o: ../src/postprocess.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.o"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.o -c /home/alientek/workspace/v4l2_displayer_test/src/postprocess.cc

CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.i"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alientek/workspace/v4l2_displayer_test/src/postprocess.cc > CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.i

CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.s"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alientek/workspace/v4l2_displayer_test/src/postprocess.cc -o CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.s

CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.o: CMakeFiles/v4l2_displayer_test.dir/flags.make
CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.o: ../src/rknn_yolov5.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.o"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.o -c /home/alientek/workspace/v4l2_displayer_test/src/rknn_yolov5.cc

CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.i"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alientek/workspace/v4l2_displayer_test/src/rknn_yolov5.cc > CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.i

CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.s"
	/opt/atk-dlrk3568-5_10_sdk-toolchain/bin/aarch64-buildroot-linux-gnu-g++ --sysroot=/home/alientek/rk3568_linux_sdk/buildroot/output/rockchip_atk_dlrk3568/host/aarch64-buildroot-linux-gnu/sysroot/ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alientek/workspace/v4l2_displayer_test/src/rknn_yolov5.cc -o CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.s

# Object files for target v4l2_displayer_test
v4l2_displayer_test_OBJECTS = \
"CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.o" \
"CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.o" \
"CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.o" \
"CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.o" \
"CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.o" \
"CMakeFiles/v4l2_displayer_test.dir/src/main.cc.o" \
"CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.o" \
"CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.o"

# External object files for target v4l2_displayer_test
v4l2_displayer_test_EXTERNAL_OBJECTS =

../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/src/buffer_manager.cc.o
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/src/camera.cc.o
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/src/drm_disp.cc.o
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/src/framebuffer.cc.o
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/src/image_converter.cc.o
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/src/main.cc.o
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/src/postprocess.cc.o
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/src/rknn_yolov5.cc.o
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/build.make
../bin/v4l2_displayer_test: ../lib/ffmpeg/lib/libavcodec.so
../bin/v4l2_displayer_test: ../lib/ffmpeg/lib/libavformat.so
../bin/v4l2_displayer_test: ../lib/ffmpeg/lib/libavutil.so
../bin/v4l2_displayer_test: ../lib/ffmpeg/lib/libswscale.so
../bin/v4l2_displayer_test: ../lib/ffmpeg/lib/libswresample.so
../bin/v4l2_displayer_test: CMakeFiles/v4l2_displayer_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Linking CXX executable ../bin/v4l2_displayer_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/v4l2_displayer_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/v4l2_displayer_test.dir/build: ../bin/v4l2_displayer_test

.PHONY : CMakeFiles/v4l2_displayer_test.dir/build

CMakeFiles/v4l2_displayer_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/v4l2_displayer_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/v4l2_displayer_test.dir/clean

CMakeFiles/v4l2_displayer_test.dir/depend:
	cd /home/alientek/workspace/v4l2_displayer_test/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alientek/workspace/v4l2_displayer_test /home/alientek/workspace/v4l2_displayer_test /home/alientek/workspace/v4l2_displayer_test/build /home/alientek/workspace/v4l2_displayer_test/build /home/alientek/workspace/v4l2_displayer_test/build/CMakeFiles/v4l2_displayer_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/v4l2_displayer_test.dir/depend

