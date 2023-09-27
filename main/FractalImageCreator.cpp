#include "FractalImageCreator.h"
#include "Mandelbrot.h"

#include <iostream>
#include <cmath>
#include <thread>



FractalImageCreator::FractalImageCreator() : palette(nIterations)
{
    // Define colors for palete
    vector<pair<float, RGB>> colors;
    // First color position must always be 0, last position - 1.
    // colors.emplace_back(pair<float, RGB>(0.0f, {255, 170, 0}));
    colors.emplace_back(pair<float, RGB>(0.0f, {0, 7, 100}));// 0, 7, 100
    colors.emplace_back(pair<float, RGB>(0.035f, {255, 170, 0}));//
    colors.emplace_back(pair<float, RGB>(0.16f, {32, 107, 203}));
    colors.emplace_back(pair<float, RGB>(0.42f, {237, 255, 255}));
    colors.emplace_back(pair<float, RGB>(0.6425f, {255, 170, 0}));
    colors.emplace_back(pair<float, RGB>(0.8575f, {0, 2, 0}));
    colors.emplace_back(pair<float, RGB>(1.0f, {100, 7, 0}));
	palette.init(colors);
}

QImage FractalImageCreator::createImage(QSize imageSize, QPointF center, double radius)
{
	QImage image(imageSize, QImage::Format_RGB32);

	double step = 2 * radius / std::min(imageSize.height(), imageSize.width());

	for (int i = 0; i < imageSize.width(); ++i)
	{
		double x = center.x() - radius + step * i;
		for (int j = 0; j < imageSize.height(); ++j)
		{
			double y = center.y() - radius + step * j;

			double zR, zI;
			int iterations = Mandelbrot::getIterations(x,y, zR, zI, nIterations);
			QRgb value;
			if (iterations < 1000)
			{
				double mod = log(zR + zI) / 2;
				double nu = log(mod / log(2)) / log(2);
				double nIt = iterations + 1 - nu;

				RGB col1 = palette[nIt];
				RGB col2 = palette[nIt+1];
				RGB col = RGB::interpolate(col1, col2, 0.0f, 1.0f, fmod(nIt, 1.0));

				value = qRgb( col.R, col.G, col.B );

			}
			else
				value = qRgb(0,0,0);

			image.setPixel(i, j, value);
		}
	}

	return image;
}

QImage FractalImageCreator::createImageT(QSize imageSize, QPointF center, double radius)
{
	QImage image(imageSize, QImage::Format_RGB32);

	double step = 2 * radius / std::min(imageSize.height(), imageSize.width());
	// FIXME: Use proper division not to get underflow in the end frame
	int nThreads = 60;// std::thread::hardware_concurrency();     // Get max thread number
	std::vector<std::thread> t(nThreads);                   // Initialize thread array
	int width = imageSize.width() / nThreads;
	int start = 0;
	double initX = center.x() - radius;
	double initY = center.y() - radius;
	for (int i=0; i<nThreads; ++i)
	{
		// Pass member function to each thread
		t[i] = std::thread(&FractalImageCreator::fillImage, this, std::ref(image), start, width, initX, initY, step);
		start += width;
	}
	// Join all threads with main
	for (int i=0; i<nThreads;  ++i) t[i].join();



	return image;
}

void FractalImageCreator::fillImage(QImage& image, int start, int width, double initX, double initY, double step)
{
	for (int i = start; i < start + width; ++i)
	{
		double x = initX + step * i;
		for (int j = 0; j < image.height(); ++j)
		{
			double y = initY + step * j;

			double zR, zI;
			int iterations = Mandelbrot::getIterations(x,y, zR, zI, nIterations);
			QRgb value;
			if (iterations < 1000)
			{
				double mod = log(zR + zI) / 2;
				double nu = log(mod / log(2)) / log(2);
				double nIt = iterations + 1 - nu;

				RGB col1 = palette[nIt];
				RGB col2 = palette[nIt+1];
				RGB col = RGB::interpolate(col1, col2, 0.0f, 1.0f, fmod(nIt, 1.0));

				value = qRgb( col.R, col.G, col.B );
			}
			else
				value = qRgb(0,0,0);

			image.setPixel(i, image.height() - 1 - j, value);
		}
	}
}


