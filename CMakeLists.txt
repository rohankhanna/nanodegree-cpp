cmake_minimum_required(VERSION 3.7)
project(tf_detector)

set(CMAKE_CXX_STANDARD 11)
set(SOURCE_FILES main.cpp src/utils.cpp src/utils.h src/WebcamStream.cpp src/Fps.cpp src/Detection.h src/VideoStream.h src/Stream.h src/VideoStream.cpp src/Stream.cpp)
add_executable(tf_detector_example ${SOURCE_FILES})
find_package(TensorflowCC REQUIRED)

find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui objdetect videoio)
include_directories("/usr/local/include/opencv4/")
target_link_libraries(tf_detector_example TensorflowCC::TensorflowCC ${OpenCV_LIBS})

# ==================== PATHS TO SPECIFY! ==================== #

# Eigen lib headers
include_directories("/usr/local/include/eigen3")
