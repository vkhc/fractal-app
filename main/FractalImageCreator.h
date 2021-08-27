#pragma once

#include <memory>
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


    unique_ptr<int[]> nIterations;
    unique_ptr<uint32_t[]> imgBuffer;
    // unique_ptr<QImage> image;

public:
    FractalImageCreator(int w, int h);
    
    void calculateIterations(QImage& image);
    inline double screenToRealX(int x) {return cX - range * ratio + range * 2 * x / screenHeight;}
    inline double screenToRealY(int y) {return cY - range + range * 2 * y / screenHeight;}
    void setDrawArea(int tlx, int tly, int brx, int bry);



};