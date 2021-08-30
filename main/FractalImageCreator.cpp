#include "FractalImageCreator.h"
#include "Mandelbrot.h"
#include <iostream>
#include <cmath>
#include <thread>




FractalImageCreator::FractalImageCreator(int w, int h) : screenWidth(w), screenHeight(h),
                                                         nIterations(new int[screenWidth * screenHeight + 1]{}),
                                                         imgBuffer(new uint32_t[screenWidth * screenHeight +1]{}) {
    if (screenHeight > screenWidth) std::swap(screenWidth, screenHeight);
    ratio = (double)screenWidth / screenHeight;


}

void FractalImageCreator::calculateIterations(QImage& image) {
    start_time = std::chrono::steady_clock::now();
    for (int i=0; i<screenWidth; ++i) {
        for (int j=0; j<screenHeight; ++j) {
            double x = screenToRealX(i);
            double y = screenToRealY(j);
            double iterations = (double)Mandelbrot::getIterations(x,y);
            nIterations[j * screenWidth + i] = iterations;    

            double t = (double)iterations / 1000;

            QRgb value;
            value = qRgb( 250*pow(t*(1-t), 0.3), 200*pow(t, 0.25), 100*pow(t, 0.25) );
            if (iterations < 1000) image.setPixel(i, j, value);
            else image.setPixel(i, j, qRgb(0,0,0));

        }  
    }
    elapsed_time = std::chrono::steady_clock::now() - start_time;
}

void FractalImageCreator::calculateIterationsThread(QImage& image) {
    startTimer();

    int nThreads = std::thread::hardware_concurrency();     // Get max thread number
    std::thread t[nThreads];                                // Initialize thread array
    int width = screenWidth / nThreads;
    int start = 0;
    int end = width;
    for (int i=0; i<nThreads; ++i) {
        // Pass member function to each thread
        t[i] = std::thread(&FractalImageCreator::iterationsInRange, this, std::ref(image), start, end);
        start += width;
        end += width;    
    }

    // Join all threads with main
    for (int i=0; i<nThreads;  ++i) t[i].join();

    stopTimer();
}

//Calculate iterations and draw pixels in screen column from "start" to "end"
void FractalImageCreator::iterationsInRange(QImage& image, int start, int end) {
    for (int i=start; i<end; ++i) {
        for (int j=0; j<screenHeight; ++j) {
            double x = screenToRealX(i);
            double y = screenToRealY(j);
            double iterations = (double)Mandelbrot::getIterations(x,y);
            nIterations[j * screenWidth + i] = iterations;    

            double t = (double)iterations / 1000;

            QRgb value;
            value = qRgb( 250*pow(t*(1-t), 0.3), 200*pow(t, 0.25), 100*pow(t, 0.25) );
            if (iterations < 1000) image.setPixel(i, j, value);
            else image.setPixel(i, j, qRgb(0,0,0));

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