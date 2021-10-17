//
// Created by rohan on 25/06/2021.
//

#include <stdexcept>
#include <iostream>
#include <memory>
#include <string>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/videoio.hpp>

#include "VideoStream.h"
#include "utils.h"

VideoStream::VideoStream() {
    this->init();
}

VideoStream::VideoStream(std::string filepath) : filepath(filepath){
    this->init();
}

void VideoStream::init() {
    // Open selected camera using selected API
    this->stream = std::shared_ptr<cv::VideoCapture>( new cv::VideoCapture(this->filepath) );

    if (!this->stream->isOpened()) {
        std::cerr << "(!)ERROR: Unable to open video file\n";
        throw "(!)ERROR: Unable to open video file\n";
    }

    *(this->stream.get()) >> this->frame;
    if (this->frame.empty()) {
        std::cerr << "(!)Error1: Blank frame grabbed\n";
        return;
    }
}


