#include "FractalImageCreator.h"
#include "Mandelbrot.h"
#include <iostream>
#include <cmath>
#include <thread>



RGB interpolate(RGB& low, RGB& high, float lpos, float hpos, float t) {
    float i = (t - lpos) / (hpos - lpos);
    RGB result = {(float)(high.R - low.R)*i + (float)low.R,
                  (float)(high.G - low.G)*i + (float)low.G,
                  (float)(high.B - low.B)*i + (float)low.B};
    return result;
}


FractalImageCreator::FractalImageCreator(int w, int h) : screenWidth(w), screenHeight(h),
                                                         colorPalete(new RGB[nIterations + 1]{}),
                                                         IterationsBuffer(new int[screenWidth * screenHeight + 1]{}),
                                                         imgBuffer(new uint32_t[screenWidth * screenHeight +1]{}) {
    if (screenHeight > screenWidth) std::swap(screenWidth, screenHeight);
    ratio = (double)screenWidth / screenHeight;

    float p0 = 0.0f, p1 = 0.02f, p2 = 0.22f, p3 = 0.6425f, p4 = 0.8575f, p5 = 1.0f;
    RGB col00 = {0, 7, 100};        // Colors defining
    RGB col16 = {66, 107, 203};     // palete
    RGB col42 = {237, 255, 255};
    RGB col64 = {255, 170, 0};
    RGB col85 = {0, 2, 0};
    RGB col;    // Color to push into palete array

    for (int i=0; i<nIterations; ++i) {
        float t = (float)i / nIterations;

        if (t <= p1) {
            col = interpolate(col00, col16, p0, p1, t);
        } else if (t > p1 && t <= p2) {
            col = interpolate(col16, col42, p1, p2, t);
        } else if (t > p2 && t <= p3) {
            col = interpolate(col42, col64, p2, p3, t);
        } else if (t > p3 && t <= p4) {
            col = interpolate(col64, col85, p3, p4, t);
        } else if (t <= p5) {
            col = interpolate(col85, col00, p4, p5, t);
        } else col = {0,0,0};

        colorPalete[i] = col;
    }
    


}

// void FractalImageCreator::calculateIterations(QImage& image) {
//     start_time = std::chrono::steady_clock::now();
//     for (int i=0; i<screenWidth; ++i) {
//         for (int j=0; j<screenHeight; ++j) {
//             double x = screenToRealX(i);
//             double y = screenToRealY(j);
//             double iterations = (double)Mandelbrot::getIterations(x,y);
//             IterationsBuffer[j * screenWidth + i] = iterations;    

//             double t = (double)iterations / 1000;

//             QRgb value;
//             value = qRgb( 250*pow(t*(1-t), 0.3), 200*pow(t, 0.25), 100*pow(t, 0.25) );
//             if (iterations < 1000) image.setPixel(i, j, value);
//             else image.setPixel(i, j, qRgb(0,0,0));

//         }  
//     }
//     elapsed_time = std::chrono::steady_clock::now() - start_time;
// }

void FractalImageCreator::calculateIterationsThread(QImage& image) {
    startTimer();

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

    stopTimer();
}

//Calculate iterations and draw pixels in screen column
void FractalImageCreator::iterationsInRange(QImage& image, int start, int range) {
    int ct = 0;
    for (int i=start; i<start+range; ++i) {
        for (int j=0; j<screenHeight; ++j) {
            double x = screenToRealX(i);
            double y = screenToRealY(j);
            double zR, zI;
            int iterations = Mandelbrot::getIterations(x,y, zR, zI, nIterations);
            IterationsBuffer[j * screenWidth + i] = iterations;
            QRgb value;    
            if (iterations < 1000) {
                double mod = log(zR + zI) / 2;
                double nu = log(mod / log(2)) / log(2);
                double nIt = iterations + 1 - nu;

                RGB col1 = colorPalete[nIt];
                RGB col2 = colorPalete[nIt+1];
                RGB col = interpolate(col1, col2, 0.0f, 1.0f, fmod(nIt, 1.0));

                // std::cout << mu << " ";
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