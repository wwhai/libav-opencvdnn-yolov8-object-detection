cmake_minimum_required(VERSION 3.5)

project(libav-opencvdnn-yolov8-object-detection LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# OpenCV_LIB_DIR
set(OpenCV_LIB_DIR "E:/opencv4.10/opencv/build/x64/vc16/lib")
set(OpenCV_LIBS
    optimized ${OpenCV_LIB_DIR}/opencv_world4100.lib
    debug ${OpenCV_LIB_DIR}/opencv_world4100d.lib
)
include_directories("E:/opencv4.10/opencv/build/include")
# LIBAV_LIB_DIR
set(LIBAV_LIB_DIR "C:/Users/wangwenhai/workspace/ffmpeg-7.0.2-full_build-shared/lib")
set(LIBAV_LIBS
  optimized   ${LIBAV_LIB_DIR}/avcodec.lib
  optimized   ${LIBAV_LIB_DIR}/avdevice.lib
  optimized   ${LIBAV_LIB_DIR}/avfilter.lib
  optimized   ${LIBAV_LIB_DIR}/avformat.lib
  optimized   ${LIBAV_LIB_DIR}/avutil.lib
  optimized   ${LIBAV_LIB_DIR}/postproc.lib
  optimized   ${LIBAV_LIB_DIR}/swresample.lib
  optimized   ${LIBAV_LIB_DIR}/swscale.lib
)
include_directories("C:/Users/wangwenhai/workspace/ffmpeg-7.0.2-full_build-shared/include")


# 添加可执行文件
add_executable(libav-opencvdnn-yolov8-object-detection main.cpp)
target_link_libraries(libav-opencvdnn-yolov8-object-detection
    ${OpenCV_LIBS}
    ${LIBAV_LIBS}
)

# 安装可执行文件
install(TARGETS libav-opencvdnn-yolov8-object-detection
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

