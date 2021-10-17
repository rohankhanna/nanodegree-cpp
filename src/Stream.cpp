//
// Created by rohan on 25/06/2021.
//

#include <iostream>
#include <mutex>
#include <memory>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>


#include <tensorflow/core/framework/tensor.h>

#include "Stream.h"
#include "Fps.h"
#include "utils.h"


// make thread stop
void Stream::stop() {
    this->stopped = true;
}

void Stream::CaptureThread(std::mutex& mutex, std::promise<int> promise) {
    cv::Mat temp;
    while(!this->is_stopped()){
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        *(this->stream.get()) >> temp;

        if (temp.empty()) {
            std::cerr << "(!)Error2: Blank frame grabbed\n";
            this->stop();
            continue;
        }
        else {
            mutex.lock();
            this->frame = temp.clone();
            mutex.unlock();
        }
    }
    promise.set_value(0);
}

bool Stream::is_stopped(){
    return this->stopped;
}


void Stream::Show(Fps &fps, std::vector<tensorflow::Tensor> &outputs, double &thresholdScore, double &thresholdIOU, cv::Mat &output_frame, std::map<int, std::string> &labelsMap, std::mutex& mutex_2_final){
    fps.start();
    while(!this->is_stopped()) {
        mutex_2_final.lock();
        if (!outputs.empty()) {
            tensorflow::TTypes<float>::Flat scores = outputs[1].flat<float>();
            tensorflow::TTypes<float>::Flat classes = outputs[2].flat<float>();
            tensorflow::TTypes<float>::Flat numDetections = outputs[3].flat<float>();
            tensorflow::TTypes<float, 3>::Tensor boxes = outputs[0].flat_outer_dims<float, 3>();
            mutex_2_final.unlock();

            if(!output_frame.empty()) {
                std::vector<size_t> goodIdxs = filterBoxes(scores, boxes, thresholdIOU, thresholdScore);
                // Draw bboxes and captions
                drawBoundingBoxesOnImage(output_frame, scores, classes, boxes, labelsMap, goodIdxs);
            }
            else
                continue;
            fps.update();
            putText(output_frame,
                    std::to_string(fps.fps()).substr(0, 5),
                    cv::Point(0, output_frame.rows),
                    cv::FONT_HERSHEY_SIMPLEX,
                    0.7,
                    cv::Scalar(255, 255, 255)
            );
        }
        else
            mutex_2_final.unlock();
        try {
            imshow("stream", output_frame);

            if (cv::waitKey(5) >= 0){
                this->stop();
                break;
            }
        }
        catch(...){
            continue;
        }

    }
}