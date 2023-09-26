#pragma once

#include <memory>
#include <chrono>
#include <QImage>

#include "ColorPalette.h"
#include "Timer.h"

class FractalImageCreator {
public:
	FractalImageCreator(int w, int h);

	QImage createImage(double fromX, double fromY, double toX, double toY, QSize imageSize);

	void calculateIterationsThread(QImage& image);
	void iterationsInRange(QImage& image, int start, int end);
	void setDrawingArea(int tlx, int tly, int brx, int bry);
	inline void adjustRange(float factor) { range *= factor; }
	void setImageCenter(int x, int y);
	void moveImageCenter(int x, int y);
	inline const double screenToRealX(int x) const { return cX - range * m_width / m_height + range * 2 * x / m_height; }
	inline const double screenToRealY(int y) const { return cY - range + range * 2 * y / m_height; }
	double frameCalcTime = 0.0;

private:
	int m_width;
	int m_height;
    int nIterations = 1000;

    double range = 1.0;
    double cX = -0.6;
    double cY = 0.0;

    ColorPalette palette;

    Timer timer;
};

