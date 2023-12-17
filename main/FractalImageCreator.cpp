#include "FractalImageCreator.h"

#include "Mandelbrot.h"

#include <iostream>
#include <cmath>
#include <thread>
#include <future>


static void wait_for_all(std::vector<std::future<void>>& futures)
{
    for (auto& f : futures) f.get();
}

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

FractalImageCreator::FractalImageCreator(int width, int height) :
	m_width(width),
	m_height(height),
	m_buffer(std::make_unique<uint32_t[]>(width*height)),
	palette(nIterations),
	pool(TPool::instance())
{
    // Define colors for palete
    vector<pair<float, RGB>> colors;
    // First color position must always be 0, last position - 1.
    // colors.emplace_back(pair<float, RGB>(0.0f, {255, 170, 0}));
    colors.emplace_back(pair<float, RGB>(0.0f,    {0, 7, 100}));// 0, 7, 100
    colors.emplace_back(pair<float, RGB>(0.035f,  {255, 170, 0}));//
    colors.emplace_back(pair<float, RGB>(0.16f,   {32, 107, 203}));
    colors.emplace_back(pair<float, RGB>(0.42f,   {237, 255, 255}));
    colors.emplace_back(pair<float, RGB>(0.6425f, {255, 170, 0}));
    colors.emplace_back(pair<float, RGB>(0.8575f, {0, 2, 0}));
    colors.emplace_back(pair<float, RGB>(1.0f,    {100, 7, 0}));
	palette.init(colors);
}

uint32_t* FractalImageCreator::createImageRaw(double cX, double cY, double radius)
{
	int nTasks = 40;

	int chunk_height = std::ceil((double)m_height / nTasks);

	double step = 2 * radius / std::min(m_width, m_height);
	double bottomLeftX = cX - radius;
	double bottomLeftY = cY - radius;

	int start = 0;

	std::vector<std::future<void>> tasks(nTasks);
	for (auto& task : tasks)
	{
		if (start + chunk_height > m_height) // trim the last chunk if it goes beyond image
		{
			chunk_height = m_height - start;
		}

		double initX = bottomLeftX;
		double initY = bottomLeftY + 2 * radius * start / m_height;
		uint32_t* point_in_buffer = m_buffer.get() + m_width * start;

		auto packaged_task = std::bind(&FractalImageCreator::fillImage, this, point_in_buffer, m_width, chunk_height, initX, initY, step);
		task = pool.async(packaged_task);

		start += chunk_height;
	}

	wait_for_all(tasks);

	return m_buffer.get();
}

void FractalImageCreator::fillImage(uint32_t* buffer, int width, int height, double initX, double initY, double step)
{
	for (int j = 0; j < height; ++j)
	{
		double Y = initY + step * j;
		for (int i = 0; i < width; ++i)
		{
			double X = initX + step * i;

			double zR, zI;
			int iterations = Mandelbrot::getIterations(X, Y, zR, zI, nIterations);
			uint32_t value;
			if (iterations < 1000)
			{
				auto col = colorFrom(iterations, zR, zI);

				value = packRGB( col.R, col.G, col.B );
			}
			else
				value = packRGB(0,0,0);

			*(buffer + width * j + i) = value;
		}
	}
}
