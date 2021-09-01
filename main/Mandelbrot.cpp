#include "Mandelbrot.h"


// int Mandelbrot::getIterations(double x0, double y0, int MAX_ITERATIONS) {
//     // x should be scaled to [-2.5 : 1] range and y to [-1 : 1] initially
//     double x, y, x2, y2;
//     x = y = x2 = y2 = 0;

//     int iterations = 0;

//     while (iterations < MAX_ITERATIONS) {
//         y = (x + x) * y + y0;
//         x = x2 - y2 + x0;
//         x2 = x * x;
//         y2 = y * y;

//         if (x2 + y2 > 4) break;

//         ++iterations;
//     }
//     return iterations;
// }

int Mandelbrot::getIterations(double x0, double y0, double& escZr, double& escZi, int MAX_ITERATIONS) {
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
    escZr = x2;
    escZi = y2;
    return iterations;
}