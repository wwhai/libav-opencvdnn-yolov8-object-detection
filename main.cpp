#include <iostream>
#include "libavenv.h"

void Handler(AVFrame *frame)
{
    std::cout << "avcodec_receive_frame.pts: " << std::endl;
}
int main(int argc, char **argv)
{
    LibAVEnv avEnv = LibAVEnv();
    RunError error1 = avEnv.InitInputCodec(std::string("rtsp://192.168.10.244:554/av0_0"));
    if (error1.Code() < 0)
    {
        std::cout << "error.Error: " << error1.Error() << std::endl;
        throw error1;
    }
    avEnv.StartStream(Handler);
    avEnv.Stop();
}

// int main(int argc, char** argv)
//{
//     const std::string modelPath = "C:/Users/wangwenhai/workspace/libav-opencvdnn-yolov8-object-detection/yolov8n.onnx";
//     cv::Mat frame = cv::imread("C:/Users/wangwenhai/workspace/libav-opencvdnn-yolov8-object-detection/1.png");
//     cv::dnn::Net net = cv::dnn::readNetFromONNX(modelPath);
//     net.setPreferableBackend(cv::dnn::DNN_BACKEND_CUDA);
//     net.setPreferableTarget(cv::dnn::DNN_TARGET_CUDA);
//     // 准备输入
//     cv::Mat blob;
//     cv::dnn::blobFromImage(frame, blob, 1.0 / 255.0, cv::Size(640, 640), cv::Scalar(), true, false);
//     // 设置网络输入
//     net.setInput(blob);
//     // 执行前向传播，获取输出
//     std::vector<cv::Mat> outs;
//     net.forward(outs, net.getUnconnectedOutLayersNames());
//     // 后处理，提取检测结果
//     postprocess(frame, outs, 0.25, 0.5);
//     // 显示结果
//     cv::imshow("YOLOv8 Detection", frame);
//     cv::waitKey(0);
//     return 0;
// }
