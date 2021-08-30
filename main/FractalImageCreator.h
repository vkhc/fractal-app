#pragma once

#include <memory>
#include <chrono>
#include <QImage>

using std::unique_ptr;

class FractalImageCreator {
public:
    int screenWidth;
    int screenHeight;

    double range = 1.0;
    double cX = -0.6;
    double cY = 0.0;
    double ratio;

    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = std::chrono::duration<double>::zero();

    unique_ptr<int[]> nIterations;
    unique_ptr<uint32_t[]> imgBuffer;

public:
    FractalImageCreator(int w, int h);
    
    void calculateIterations(QImage& image);
    void calculateIterationsThread(QImage& image);
    inline double screenToRealX(int x) {return cX - range * ratio + range * 2 * x / screenHeight;}
    inline double screenToRealY(int y) {return cY - range + range * 2 * y / screenHeight;}
    void setDrawingArea(int tlx, int tly, int brx, int bry);

    void iterationsInRange(QImage& image, int start, int end);


};