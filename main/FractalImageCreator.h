#pragma once

#include <memory>
#include <chrono>
#include <QImage>

using std::unique_ptr;

struct RGB {
    uint8_t R;
    uint8_t G;
    uint8_t B;

    static RGB interpolate(RGB& low, RGB& high, float lpos, float hpos, float t);
};

class FractalImageCreator {
private:
    int screenWidth;
    int screenHeight;
    int nIterations = 1000;

    double range = 1.0;
    double cX = -0.6;
    double cY = 0.0;
    double ratio;

    unique_ptr<int[]> IterationsBuffer;
    unique_ptr<uint32_t[]> imgBuffer;
    unique_ptr<RGB[]> colorPalete;

public:
    FractalImageCreator(int w, int h);
    void initColorPalete();
    
    void calculateIterationsThread(QImage& image);
    void iterationsInRange(QImage& image, int start, int end);
    void setDrawingArea(int tlx, int tly, int brx, int bry);
    inline double getElapsedTime() { return elapsed_time.count(); }
    inline void adjustRange(float factor) { range *= factor; }
    void setImageCenter(int x, int y);

private:
    inline double screenToRealX(int x) { return cX - range * ratio + range * 2 * x / screenHeight; }
    inline double screenToRealY(int y) { return cY - range + range * 2 * y / screenHeight; }
    

private:    // For calculating time of fractal generation
    std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed_time = std::chrono::duration<double>::zero();
    inline void startTimer() { start_time = std::chrono::steady_clock::now(); }
    inline void stopTimer() { elapsed_time = std::chrono::steady_clock::now() - start_time; }

};

