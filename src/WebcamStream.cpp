#include <iostream>
#include <mutex>

//#include <opencv2/opencv.hpp>
//#include <opencv2/videoio.hpp>

#include "WebcamStream.h"
#include "utils.h"

WebcamStream::WebcamStream() {
    this->init();
}

WebcamStream::WebcamStream(int device, int api)
: device_id(device), apiID(api) {
    this->init();
}

void WebcamStream::init() {
    // Open selected camera using selected API
//    this->stream->open(this->device_id, this->apiID);
    this->stream = std::shared_ptr<cv::VideoCapture>( new cv::VideoCapture( this->device_id, this->apiID ) );
    if (!this->stream->isOpened()) {
        std::cerr << "(!)ERROR: Unable to open camera\n";
        exit(EXIT_FAILURE);
    }

    this->stream->read(this->frame);
    if (this->frame.empty()) {
        std::cerr << "(!)Error1: Blank frame grabbed\n";
        return;
    }
}


