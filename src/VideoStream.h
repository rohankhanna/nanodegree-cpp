//
// Created by rohan on 24/06/2021.
//

#ifndef TF_DETECTOR_VIDEOSTREAM_H
#define TF_DETECTOR_VIDEOSTREAM_H

#include <mutex>
#include <future>
#include <string>
#include <memory>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/videoio.hpp>


#include <tensorflow/core/lib/io/path.h>
#include <tensorflow/core/util/command_line_flags.h>
#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/core/public/session.h>

#include "Fps.h"
#include "Stream.h"

class VideoStream : public Stream {
private:
    void init();
    std::string filepath;

public:
    VideoStream();
    VideoStream(std::string filepath);

};


#endif //TF_DETECTOR_VIDEOSTREAM_H
