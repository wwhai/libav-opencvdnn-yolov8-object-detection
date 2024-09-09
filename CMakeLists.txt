cmake_minimum_required(VERSION 3.5)

project(libav-opencvdnn-yolov8-object-detection LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 查找 FFmpeg
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFMPEG REQUIRED libavcodec libavformat libavutil libswscale)

# 添加可执行文件
add_executable(libav-opencvdnn-yolov8-object-detection
    main.cpp
    LibAVEnv.cpp
    DnnRunner.cpp
    RunError.cpp
    OnnxStreamMuxer.cpp
    StreamMuxer.cpp
)

# 链接库
target_link_libraries(libav-opencvdnn-yolov8-object-detection
    ${OpenCV_LIBS}
    ${FFMPEG_LIBRARIES}
)

# 安装可执行文件
install(TARGETS libav-opencvdnn-yolov8-object-detection
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
