#include "FractalImageCreator.h"
#include "Mandelbrot.h"

#include <iostream>
#include <cmath>
#include <thread>



FractalImageCreator::FractalImageCreator(int w, int h) : screenWidth(w), screenHeight(h),
                                                         palette(nIterations),
                                                         IterationsBuffer(new int[screenWidth * screenHeight + 1]{}),
                                                         imgBuffer(new uint32_t[screenWidth * screenHeight +1]{}) {

    if (screenHeight > screenWidth) std::swap(screenWidth, screenHeight);
    ratio = (double)screenWidth / screenHeight;

    // Define colors for palete
    vector<pair<float, RGB>> colors;
    // First color position must always be 0, last position - 1.
    // colors.emplace_back(pair<float, RGB>(0.0f, {255, 170, 0}));
    colors.emplace_back(pair<float, RGB>(0.0f, {0, 7, 100}));// 0, 7, 100
    colors.emplace_back(pair<float, RGB>(0.035f, {255, 170, 0}));//
    colors.emplace_back(pair<float, RGB>(0.16f, {32, 107, 203}));
    colors.emplace_back(pair<float, RGB>(0.42f, {237, 255, 255}));
    colors.emplace_back(pair<float, RGB>(0.6425f, {255, 170, 0}));
    colors.emplace_back(pair<float, RGB>(0.8575f, {0, 2, 0}));
    colors.emplace_back(pair<float, RGB>(1.0f, {100, 7, 0}));
    palette.init(colors);
}

void FractalImageCreator::calculateIterationsThread(QImage& image) {
    timer.startTimer();

    int nThreads = std::thread::hardware_concurrency();     // Get max thread number
    std::thread t[nThreads];                                // Initialize thread array
    int width = screenWidth / nThreads;
    int start = 0;
    for (int i=0; i<nThreads; ++i) {
        // Pass member function to each thread
        t[i] = std::thread(&FractalImageCreator::iterationsInRange, this, std::ref(image), start, width);
        start += width; 
    }
    // Join all threads with main
    for (int i=0; i<nThreads;  ++i) t[i].join();

    timer.stopTimer();
    frameCalcTime = timer.getElapsedTime();
}

//Calculate iterations and draw pixels in a screen column
void FractalImageCreator::iterationsInRange(QImage& image, int start, int range) {
    for (int i=start; i<start+range; ++i) {
        for (int j=0; j<screenHeight; ++j) {
            double x = screenToRealX(i);
            double y = screenToRealY(j);
            double zR, zI;
            int iterations = Mandelbrot::getIterations(x,y, zR, zI, nIterations);
            // IterationsBuffer[j * screenWidth + i] = iterations;
            QRgb value;    
            if (iterations < 1000) {
                double mod = log(zR + zI) / 2;
                double nu = log(mod / log(2)) / log(2);
                double nIt = iterations + 1 - nu;

                RGB col1 = palette[nIt]; 
                RGB col2 = palette[nIt+1]; 
                RGB col = RGB::interpolate(col1, col2, 0.0f, 1.0f, fmod(nIt, 1.0));

                value = qRgb( col.R, col.G, col.B );

            } else value = qRgb(0,0,0);
            image.setPixel(i, j, value);
        }  
    }
}

void FractalImageCreator::setDrawingArea(int tlx, int tly, int brx, int bry) {
        double sx = screenToRealX(tlx);
        double ex = screenToRealX(brx);
        double sy = screenToRealY(tly);
        double ey = screenToRealY(bry);
        double r = (ey - sy)/2;
        double x = (ex + sx) /2;
        double y = (ey + sy) /2;
        range = std::abs(r);
        cX = x;
        cY = y;
 }

void FractalImageCreator::setImageCenter(int x, int y) {
    cX = screenToRealX(x);
    cY = screenToRealY(y);
}

void FractalImageCreator::moveImageCenter(int dx, int dy) {
    double moveSize = range /(screenHeight/2);
    cX += moveSize * dx;
    cY += moveSize * dy;
}



