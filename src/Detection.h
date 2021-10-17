//
// Created by rohan on 19/06/2021.
//

#ifndef TF_DETECTOR_EXAMPLE_DETECTION_H
#define TF_DETECTOR_EXAMPLE_DETECTION_H
#include <fstream>
#include <utility>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>

#include <tensorflow/core/lib/io/path.h>
#include <tensorflow/core/util/command_line_flags.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "Stream.h"

#include "utils.h"

int DetectorFunction(Stream &ws, std::vector<Tensor> &outputs, cv::Mat &output_frame, tensorflow::TensorShape &shape, string &inputLayer, std::vector<string> &outputLayer, Status &readTensorStatus, std::map<int, std::string> &labelsMap, string &ROOTDIR, string &LABELS, string &GRAPH, std::mutex& mutex_1_2, std::mutex& mutex_2_final, std::promise<int> promise){

        Tensor tensor;
        // Load and initialize the model from .pb file
        std::unique_ptr<tensorflow::Session> session;

        tensorflow::SessionOptions sess_options_;
        sess_options_.config.set_use_per_session_threads(false);
        sess_options_.config.set_intra_op_parallelism_threads(1);
        sess_options_.config.set_inter_op_parallelism_threads(1);
        session.reset(tensorflow::NewSession(sess_options_));
        string graphPath = tensorflow::io::JoinPath(ROOTDIR, GRAPH);

        LOG(INFO) << "graphPath:" << graphPath;
        Status loadGraphStatus = loadGraph(graphPath, *(session.get()) );
        if (!loadGraphStatus.ok()) {
            LOG(ERROR) << "loadGraph(): ERROR" << loadGraphStatus;
            ws.stop();
            return -1;
        } else
            LOG(INFO) << "loadGraph(): frozen graph loaded" << std::endl;

        Status readLabelsMapStatus = readLabelsMapFile(tensorflow::io::JoinPath(ROOTDIR, LABELS), labelsMap);
        if (!readLabelsMapStatus.ok()) {
            LOG(ERROR) << "readLabelsMapFile(): ERROR" << loadGraphStatus;
            ws.stop();
            return -1;
        } else
            LOG(INFO) << "readLabelsMapFile(): labels map loaded with " << labelsMap.size() << " label(s)" << std::endl;

        cv::Mat frame;
        while(!ws.is_stopped()) {
            mutex_1_2.lock();
            frame = ws.frame;
            mutex_1_2.unlock();
            // Convert mat to tensor
            tensor = Tensor(tensorflow::DT_FLOAT, shape);
            readTensorStatus = readTensorFromMat(frame, tensor);

            if (!readTensorStatus.ok()) {
                LOG(ERROR) << "Mat->Tensor conversion failed: " << readTensorStatus;
                ws.stop();
                return -1;
            }
            // Run the graph on tensor
            mutex_2_final.lock();
            Status runStatus = session->Run({{inputLayer, tensor}}, outputLayer, {}, &outputs);
            mutex_2_final.unlock();
            output_frame = frame.clone();


            if (!runStatus.ok()) {
                LOG(ERROR) << "Running model failed: " << runStatus;
                ws.stop();
                return -1;
            }
        }
        promise.set_value(0);
        return 0;
    }
#endif //TF_DETECTOR_EXAMPLE_DETECTION_H
