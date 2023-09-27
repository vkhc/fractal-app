#pragma once

#include <memory>
#include <chrono>
#include <QImage>

#include "ColorPalette.h"

class FractalImageCreator {
public:
	FractalImageCreator();

	QImage createImage(QSize imageSize, QPointF center, double radius);
	QImage createImageT(QSize imageSize, QPointF center, double radius);
	void fillImage(QImage& image, int start, int end, double initX, double initY, double step);

	void calculateIterationsThread(QImage& image);
	void iterationsInRange(QImage& image, int start, int end);


private:

    int nIterations = 1000;


    ColorPalette palette;

};

