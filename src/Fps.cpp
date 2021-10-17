#include <chrono>
#include "Fps.h"

void Fps::start() {
    this->timeStart = clock();
}

void Fps::update() {
    this->numFrames++;
}

double Fps::elapsed() {
    double elapsed_secs = double(clock() - this->timeStart) / CLOCKS_PER_SEC;
    return elapsed_secs;
}

double Fps::fps() {
    double fps = this->numFrames / this->elapsed();
    return fps;
}