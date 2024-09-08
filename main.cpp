#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>

// YOLOv8 ONNX模型路径
const std::string modelPath = "yolov8.onnx";

// 类别标签
const std::vector<std::string> classNames = {
    "person", "bicycle", "car", "motorbike", "aeroplane", "bus", "train", "truck", "boat",
    "traffic light", "fire hydrant", "stop sign", "parking meter", "bench", "bird",
    "cat", "dog", "horse", "sheep", "cow", "elephant", "bear", "zebra", "giraffe",
    "backpack", "umbrella", "handbag", "tie", "suitcase", "frisbee", "skis", "snowboard",
    // Add the remaining YOLOv8 class names here
};

void drawPred(int classId, float conf, int left, int top, int right, int bottom, cv::Mat& frame) {
    // 绘制边界框
    cv::rectangle(frame, cv::Point(left, top), cv::Point(right, bottom), cv::Scalar(0, 255, 0), 2);
    // 标签显示
    std::string label = cv::format("%.2f", conf);
    if (!classNames.empty()) {
        label = classNames[classId] + ": " + label;
    }

    // 标签绘制
    int baseLine;
    cv::Size labelSize = cv::getTextSize(label, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);
    top = std::max(top, labelSize.height);
    cv::rectangle(frame, cv::Point(left, top - labelSize.height),
                  cv::Point(left + labelSize.width, top + baseLine), cv::Scalar(255, 255, 255), cv::FILLED);
    cv::putText(frame, label, cv::Point(left, top), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
}
struct BestResult {
    int bestId;
    float bestScore;
};

BestResult getBestFromConfidenceValue(float confidenceValues[], size_t size) {
    BestResult result;
    result.bestId = -1; // 初始化为无效值
    result.bestScore = 0.0f;
    for (size_t i = 0; i < size; ++i) {
        if (confidenceValues[i] > result.bestScore) {
            result.bestId = static_cast<int>(i);
            result.bestScore = confidenceValues[i];
        }
    }
    return result;
}

void postprocess(cv::Mat& frame, const std::vector<cv::Mat>& outs, float confThreshold, float nmsThreshold) {

    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;
    // 网络输出的后处理
    int columns = 84;
    int rows = 8400;
    for (const auto& out : outs) {
        float* data_ptr = (float*)out.data;
        std::cout <<"out.rows: "<< out.size << std::endl;
        for (int i = 0; i < rows; ++i) {
            auto x = (data_ptr[i+rows*0]);
            auto y = (data_ptr[i+rows*1]);
            auto w = (data_ptr[i+rows*2]);
            auto h = (data_ptr[i+rows*3]);
            float confidenceValues[80] ={};
            for (int j = 4; j < columns; ++j) {
                confidenceValues[j-4]=data_ptr[i+rows*j];
            }
            BestResult result = getBestFromConfidenceValue(confidenceValues, 80);
            classIds.push_back(result.bestId);
            confidences.push_back(result.bestScore);
            boxes.push_back(cv::Rect(int(x-w/2), int(y-h/2) , w, h));
//            std::cout <<"x="<<x <<", y="<<y <<", w="<<w <<", h="<<h << std::endl;
        }

    }

    // 非极大值抑制
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, confThreshold, nmsThreshold, indices);
    for (int idx : indices) {
        cv::Rect box = boxes[idx];
        drawPred(classIds[idx], confidences[idx], box.x, box.y, box.x + box.width, box.y + box.height, frame);
    }
}

int main(int argc, char** argv) {
    const std::string modelPath = "C:/Users/wangwenhai/workspace/libav-opencvdnn-yolov8-object-detection/yolov8n.onnx";
    cv::Mat frame = cv::imread("C:/Users/wangwenhai/workspace/libav-opencvdnn-yolov8-object-detection/1.png");
    cv::dnn::Net net = cv::dnn::readNetFromONNX(modelPath);

    // 准备输入
    cv::Mat blob;
    cv::dnn::blobFromImage(frame, blob, 1.0 / 255.0, cv::Size(640, 640), cv::Scalar(), true, false);

    // 设置网络输入
    net.setInput(blob);

    // 执行前向传播，获取输出
    std::vector<cv::Mat> outs;
    net.forward(outs, net.getUnconnectedOutLayersNames());

    // 后处理，提取检测结果
    postprocess(frame, outs, 0.5, 0.4);

    // 显示结果
    cv::imshow("YOLOv8 Detection", frame);
    cv::waitKey(0);

    return 0;
}
