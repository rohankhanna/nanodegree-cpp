//
// Created by rohan on 24/06/2021.
//

#ifndef TF_DETECTOR_STREAM_H
#define TF_DETECTOR_STREAM_H

#include <mutex>
#include <future>
#include <memory>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/videoio.hpp>


#include <tensorflow/core/lib/io/path.h>
#include <tensorflow/core/util/command_line_flags.h>
#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/core/public/session.h>
#include "Fps.h"

class Stream {
protected:
    bool stopped = false;

public:
    cv::Mat frame;
    std::shared_ptr<cv::VideoCapture> stream;
    void CaptureThread(std::mutex& mutex, std::promise<int> promise);
    void Show(Fps &fps, std::vector<tensorflow::Tensor> &outputs, double &thresholdScore, double &thresholdIOU, cv::Mat &output_frame, std::map<int, std::string> &labelsMap, std::mutex& mutex_2_final);
    virtual void init() = 0; // to be called by constructor and initialized as per requirement
    void stop();
    bool is_stopped();

};

#endif //TF_DETECTOR_STREAM_H
