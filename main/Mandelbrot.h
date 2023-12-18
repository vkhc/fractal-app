#pragma once

namespace Mandelbrot {
    struct Result
    {
        int n_iteration;
        double escZr;
        double escZi;
    };

    Result getIterations(double x, double y, int iterations);
};
