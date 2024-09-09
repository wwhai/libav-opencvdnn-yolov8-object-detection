#ifndef DNNRUNNER_H
#define DNNRUNNER_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>

typedef struct  {
    int bestId;
    float bestScore;
} BestResult;

class DnnRunner {
private:
    std::string modelPath;
    std::vector<std::string> classNames;
    cv::dnn::Net dnnNet;

public:
    DnnRunner();
};

#endif // DNNRUNNER_H
