#pragma once

#include <memory>
#include <chrono>
#include <QImage>

#include "ColorPalette.h"
#include "Timer.h"

using std::unique_ptr;



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

    ColorPalette palette;

    Timer timer;

public:
    FractalImageCreator(int w, int h);
    
    void calculateIterationsThread(QImage& image);
    void iterationsInRange(QImage& image, int start, int end);
    void setDrawingArea(int tlx, int tly, int brx, int bry);
    inline void adjustRange(float factor) { range *= factor; }
    void setImageCenter(int x, int y);
    void moveImageCenter(int x, int y);
    inline const double screenToRealX(int x) const { return cX - range * ratio + range * 2 * x / screenHeight; }
    inline const double screenToRealY(int y) const { return cY - range + range * 2 * y / screenHeight; }
    double frameCalcTime = 0.0;
};

