#pragma once

#include <memory>
#include <chrono>
#include <QImage>

#include "ColorPalette.h"

class FractalImageCreator
{
public:
	FractalImageCreator();

	QImage createImageT(QSize imageSize, QPointF center, double radius);
	std::unique_ptr<uint32_t[]> createImageT(int width, int height, double cX, double cY, double radius);

	void fillImage(QImage& image, int start, int width, double initX, double initY, double step);
	void fillImageB(uint32_t* buffer, int height, int width, double initX, double initY, double radius);

private:
	RGB colorFrom(int iterations, double zR, double zI);

private:
    int nIterations = 1000;
    ColorPalette palette;
};

