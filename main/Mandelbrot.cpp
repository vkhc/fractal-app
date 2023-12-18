#include "Mandelbrot.h"

Mandelbrot::Result Mandelbrot::getIterations(double x0, double y0, int MAX_ITERATIONS) {
    double x, y, x2, y2;
    x = y = x2 = y2 = 0;

    int iterations = 0;

    while (iterations < MAX_ITERATIONS) {
        y = (x + x) * y + y0;
        x = x2 - y2 + x0;
        x2 = x * x;
        y2 = y * y;

        if (x2 + y2 > 4) break;

        ++iterations;
    }
    
    return { iterations, x2, y2 };
}