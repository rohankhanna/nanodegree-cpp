#ifndef OPENCV_UTILS_H
#define OPENCV_UTILS_H

#include <ctime>

class Fps
{
private:
    clock_t timeStart, timeStop;
    int numFrames = 0;
public:
    void start();
    void update();
    double elapsed();
    double fps();
};


#endif