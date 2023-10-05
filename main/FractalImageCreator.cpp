#include "FractalImageCreator.h"
#include "Mandelbrot.h"
#include "qdebug.h"

#include <iostream>
#include <cmath>
#include <thread>

RGB FractalImageCreator::colorFrom(int iterations, double zR, double zI)
{
	double mod = log(zR + zI) / 2;
	double nu = log(mod / log(2)) / log(2);
	double nIt = iterations + 1 - nu;

	RGB col1 = palette[nIt];
	RGB col2 = palette[nIt+1];
	RGB col = RGB::interpolate(col1, col2, 0.0f, 1.0f, fmod(nIt, 1.0));

	return col;
}

std::vector<std::pair<int, int>> intervals(int range, int divisor)
{
	int step = std::ceil((double)range / divisor);

	std::vector<std::pair<int, int>> result;
	for (int start = 0; start < range; start += step)
	{
		int end = start + step;
		if (end > range)
			end = range - start;

		result.emplace_back(start, end);
	}

	return result;
}

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

QImage FractalImageCreator::createImageT(QSize imageSize, QPointF center, double radius)
{
	QImage image(imageSize, QImage::Format_RGB32);

	double step = 2 * radius / std::min(imageSize.height(), imageSize.width());
	// FIXME: Use proper division not to get underflow in the end frame
	int nThreads = 30;// std::thread::hardware_concurrency();     // Get max thread number
	int width = std::ceil((double)imageSize.width() / nThreads);

	int start = 0;
	double initX = center.x() - radius;
	double initY = center.y() - radius;

//	std::vector<std::thread> t(nThreads);                   // Initialize thread array
//	for (int i=0; i<nThreads; ++i)
//	{
//		if (start + width > imageSize.width())
//		{
//			width = imageSize.width() - start;
//		}
//		// Pass member function to each thread
//		t[i] = std::thread(&FractalImageCreator::fillImage, this, std::ref(image), start, width, initX, initY, step);
//		start += width;
//	}
//	// Join all threads with main
//	for (int i=0; i<nThreads;  ++i) t[i].join();

	fillImage(image, start, imageSize.width(), initX, initY, step);

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
				auto col = colorFrom(iterations, zR, zI);

				value = qRgb( col.R, col.G, col.B );
			}
			else
				value = qRgb(0,0,0);

			image.setPixel(i, image.height() - 1 - j, value);
		}
	}
}

std::unique_ptr<uint32_t[]> FractalImageCreator::createImageT(int W, int H, double cX, double cY, double radius)
{
	auto image = std::make_unique<uint32_t[]>(W * H);

	double step = 2 * radius / std::min(W, H);
	// FIXME: Use proper division not to get underflow in the end frame
#if Threads
	int nThreads = 2;// std::thread::hardware_concurrency();     // Get max thread number
	std::vector<std::thread> t(nThreads);                   // Initialize thread array
	int height_t = std::ceil((double)H / nThreads);

	int start = 0;
	double initX = cX - radius;
	double initY = cY - radius;
	for (int i=0; i<nThreads; ++i)
	{
		if (start + height_t > H)
		{
			height_t = H - start;
		}
		// Pass member function to each thread
		t[i] = std::thread(&FractalImageCreator::fillImage, this, std::ref(image), start, height_t, initX, initY, step);
		start += height_t;
	}
	// Join all threads with main
	for (int i=0; i<nThreads;  ++i) t[i].join();
#elif old_way ||0
	double initX = cX - radius;
	double initY = cY - radius;
	fillImageB(image.get() + W*H-W, W, H, initX, initY, step);
#else
	double initX = cX - radius;
	double initY = cY - radius;
	fillImageB2(image.get(), W, H, initX, initY, step);
#endif

	return std::move(image);
}

void FractalImageCreator::fillImageB(uint32_t* buffer, int width, int height, double initX, double initY, double step)
{
	for (int i = 0; i < width; ++i)
	{
		double X = initX + step * i;
		for (int j = 0; j < height; ++j)
		{
			double Y = initY + step * j;

			double zR, zI;
			int iterations = Mandelbrot::getIterations(X, Y, zR, zI, nIterations);
			QRgb value;
			if (iterations < 1000)
			{
				auto col = colorFrom(iterations, zR, zI);

				value = qRgb( col.R, col.G, col.B );
			}
			else
				value = qRgb(0,0,0);

			if (Y > 0)
				value = qRgb(255,0,0);

			*(buffer - width * j + i) = value;
		}
	}
}

void FractalImageCreator::fillImageB2(uint32_t* buffer, int width, int height, double initX, double initY, double step)
{
//	double step = 2 * radius / std::min(width, height);

	uint32_t* pos = buffer + (width - 1) * height; // invert y

	for (int j = 0; j < height; ++j)
	{
		double Y = initY + step * j;
		for (int i = 0; i < width; ++i)
		{
			double X = initX + step * i;

			double zR, zI;
			int iterations = Mandelbrot::getIterations(X, Y, zR, zI, nIterations);
			QRgb value;
			if (iterations < 1000)
			{
				auto col = colorFrom(iterations, zR, zI);

				value = qRgb( col.R, col.G, col.B );
			}
			else
				value = qRgb(0,0,0);

			*(pos - width * j + i) = value; // invert y
		}
	}
}
