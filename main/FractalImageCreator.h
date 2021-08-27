#pragma once

#include <memory>
#include <QImage>

using std::unique_ptr;

class FractalImageCreator {
public:
    int width;
    int height;

    double x;
    double y;

    double range = 1.0;
    double cX = -0.6;
    double cY = 0.0;
    double ratio;


    unique_ptr<int[]> nIterations;
    unique_ptr<uint32_t[]> imgBuffer;
    // unique_ptr<QImage> image;

public:
    FractalImageCreator(int w, int h);
    
    void calculateIterations(QImage& image);
    void calculateIterations2(QImage& image);

    void evalRange();

    double screenToRealX(int x);
    double screenToRealY(int y);



};