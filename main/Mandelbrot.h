#pragma once

struct Mandelbrot {
    // static int getIterations(double x, double y, int iterations = 1000);
    static int getIterations(double x, double y, double& escZr, double& escZi, int iterations = 1000);

};