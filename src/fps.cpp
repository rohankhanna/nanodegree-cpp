#include <chrono>
#include "fps.h"

void FPS::start() {
    this->timeStart = clock();
}

void FPS::update() {
    this->numFrames++;
}

double FPS::elapsed() {
    double elapsed_secs = double(clock() - this->timeStart) / CLOCKS_PER_SEC;
    return elapsed_secs;
}

double FPS::fps() {
    double fps = this->numFrames / this->elapsed();
    return fps;
}
