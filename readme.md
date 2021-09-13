# Mandelbrot set fractal using c++ and Qt6
Multithreaded application for rendering Mandelbrot set. Since application computation heavy,
it can be quite laggy on lower end CPUs. Neverthless, pretty convenient way to explore fractal.

![Screenshot 0](https://github.com/vkhc/fractal-app/blob/master/screenshots/scrX.png)
![Screenshot 1](https://github.com/vkhc/fractal-app/blob/master/screenshots/scr1.png)
![Screenshot 2](https://github.com/vkhc/fractal-app/blob/master/screenshots/scr2.png)
![Screenshot 3](https://github.com/vkhc/fractal-app/blob/master/screenshots/scr3.png)



## Features
Image can be dragged using mouse movement with left button clicked.
Zoom in and out using Mouse wheel
Zoom into selected area using mouse with right button clicked.

will add high resolution image export feature sometime.

## How to build
1. Set QT_CONFIG_PATH to lib/cmake directory of Qt in the main CMakeLists.txt
2. Go to build directory
3. Configure cmake by running `cmake ..`
4. Build the project by running `cmake --build .`
5. Copy media folder to the location of executable.

ON WINDOWS: You will probably need to copy Qt DLLs to executable folder as well.
