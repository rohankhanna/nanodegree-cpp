#ifndef WEBCAMSTREAM_H
#define WEBCAMSTREAM_H

#include <mutex>
#include <future>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio/videoio_c.h>
#include <opencv2/videoio.hpp>

#include <tensorflow/core/lib/io/path.h>
#include <tensorflow/core/util/command_line_flags.h>
#include <tensorflow/core/framework/tensor.h>
#include <tensorflow/core/public/session.h>
#include "Fps.h"
#include "Stream.h"

class WebcamStream : public Stream {
private:
    int apiID = cv::CAP_ANY; // 0 = autodetect default API
    int device_id = 0;


public:
    void init();
    WebcamStream();
    WebcamStream(int device, int api);
};

#endif