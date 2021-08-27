#include "FractalImageCreator.h"
#include "Mandelbrot.h"
#include <iostream>
#include <cmath>




FractalImageCreator::FractalImageCreator(int w, int h) : width(w), height(h),
                                                         nIterations(new int[width * height + 1]{}),
                                                         imgBuffer(new uint32_t[width * height +1]{}) {
    ratio = (double)width / height;

}

void FractalImageCreator::calculateIterations(QImage& image) {
    for (int i=0; i<width; ++i) {
        for (int j=0; j<height; ++j) {
            double x = -1 + 2.0 * i / height - 0.75;
            double y = -1 + 2.0 * j / height;
            int iterations = Mandelbrot::getIterations(x,y);
            nIterations[j * width + i] = iterations;

            QRgb value;

            if (iterations >= 1000) {
                imgBuffer[j*width +i]=  0xFFFFFFFF;

                value = qRgb(100,200,0);
                image.setPixel(i, j, value);
            } 
            else  {
                imgBuffer[j*width+i] = 0;
                value = qRgb(0,0,0);
                image.setPixel(i,j,value);
            }

            
        }
    }
}


void FractalImageCreator::calculateIterations2(QImage& image) {
    evalRange(); // Update range value
    for (int i=0; i<width; ++i) {
        for (int j=0; j<height; ++j) {
            double x = screenToRealX(i);
            double y = screenToRealY(j);
            double iterations = (double)Mandelbrot::getIterations(x,y);
            nIterations[j * width + i] = iterations;    



            double t = (double)iterations / 1000;

            QRgb value;
            value = qRgb( 250*pow(t*(1-t), 0.3), 200*pow(t, 0.25), 100*pow(t, 0.25) );
            if (iterations < 1000) image.setPixel(i, j, value);
            else image.setPixel(i, j, qRgb(0,0,0));

        }  
    }
}

void FractalImageCreator::evalRange() {
    // range = std::min(xEnd - xStart, yEnd -yStart);
}

double FractalImageCreator::screenToRealX(int x) {
    return cX - range * ratio + range * 2 * x / height;
}

double FractalImageCreator::screenToRealY(int y) {
    return cY - range + range * 2 * y / height;
}