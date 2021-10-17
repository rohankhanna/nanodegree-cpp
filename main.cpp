#include <vector>
#include <iostream>
#include <thread>
#include <future>
#include <mutex>


#include "tensorflow/core/lib/io/path.h"
#include "tensorflow/core/util/command_line_flags.h"

#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>

#include "src/WebcamStream.h"
#include "src/VideoStream.h"
#include "src/Stream.h"
#include "src/Detection.h"
#include "src/Fps.h"

using tensorflow::Flag;
using tensorflow::Tensor;
using tensorflow::Status;
using tensorflow::string;
using tensorflow::int32;

using namespace std;
using namespace cv;

int main() {

    // Set dirs variables
    string ROOTDIR = "../";
    string LABELS = "model/labels_map.pbtxt";
    string GRAPH = "model/frozen_inference_graph.pb";

    // Set input & output nodes names
    string inputLayer = "image_tensor:0";
    vector<string> outputLayer = {"detection_boxes:0", "detection_scores:0", "detection_classes:0", "num_detections:0"};

    // Load labels map from .pbtxt file
    std::map<int, std::string> labelsMap = std::map<int,std::string>();

    double thresholdScore = 0.8;
    double thresholdIOU = 0.9;



    cout << "Please choose one of 2 working modes to run this application in:" <<endl;
    cout << "webcam" << endl;
    cout << "video" << endl;
    std::string choice;
    std::shared_ptr<Stream> streamObj;

    do
    {
        cin >> choice;
        if ( choice != "webcam" && choice != "video" )
        {
            cout << "Wrong choice--try again: ";
        }
        else if( choice == "webcam")
            streamObj = std::shared_ptr<WebcamStream>(new WebcamStream());
        else if( choice == "video"){
//            streamObj = std::shared_ptr<VideoStream>(new VideoStream());
            cout << "Please enter the filename relative to the current binary:" <<endl;
            std::string filename;
                cin >> filename;
                try {
                    streamObj = std::shared_ptr<VideoStream>(new VideoStream(filename));
                }
                catch (...){
                    std::cout << "Filename Error. Please check whether the file provided is a valid file path" << std::endl;
                }

        }
    } while ( choice != "webcam" && choice != "video" );
    std::cout << "Please press any key to end the detection system" << std::endl;

    Fps fps;
    cv::Mat display_frame;
    std::vector<Tensor> outputs;
    cv::Mat output_frame;

    auto streamObj_ = *(streamObj->stream.get());
    tensorflow::TensorShape shape = tensorflow::TensorShape();
    shape.AddDim(1);
    shape.AddDim((int64)streamObj_.get(CAP_PROP_FRAME_HEIGHT));
    shape.AddDim((int64)streamObj_.get(CAP_PROP_FRAME_WIDTH));
    shape.AddDim(3);
    Status readTensorStatus;

    std::promise<int> th1Promise;
    std::future<int> th1Future = th1Promise.get_future();
    std::mutex mutex_1_2;
    std::thread th1(&Stream::CaptureThread, std::ref(*(streamObj.get())), std::ref(mutex_1_2), std::move(th1Promise));

    std::promise<int> th2Promise;
    std::future<int> th2Future = th2Promise.get_future();
    std::mutex mutex_2_final;

    std::thread th2(&DetectorFunction,
        std::ref( *(streamObj.get()) ),
        std::ref(outputs),
        std::ref(output_frame),
        std::ref(shape),
        std::ref(inputLayer),
        std::ref(outputLayer),
        std::ref(readTensorStatus),
        std::ref(labelsMap),
        std::ref(ROOTDIR),
        std::ref(LABELS),
        std::ref(GRAPH),
        std::ref(mutex_1_2),
        std::ref(mutex_2_final),
        std::move(th2Promise)
    );

    streamObj->Show(fps,
            outputs,
            thresholdScore,
            thresholdIOU,
            output_frame,
            labelsMap,
            mutex_2_final
    );

    // cleanup
    cv::destroyAllWindows();
    th1.join();
    th2.join();
    return 0;
}
