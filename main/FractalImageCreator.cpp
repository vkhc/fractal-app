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

	RGB col1 = palette[(int)nIt];
	RGB col2 = palette[(int)nIt+1];
	RGB col = RGB::interpolate(col1, col2, 0.0f, 1.0f, fmod(nIt, 1.0));

	return col;
}

FractalImageCreator::FractalImageCreator(int width, int height) :
	m_width(width),
	m_height(height),
	m_buffer(std::make_unique<uint32_t[]>(width*height)),
	palette(nIterations),
	pool(thread_pool::instance())
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
	double step = 2 * radius / std::min(m_width, m_height);
	double bottomLeftX = cX - radius;
	double bottomLeftY = cY - radius;

	std::vector<std::future<void>> tasks;
	tasks.reserve(m_height); // Compute every row concurrently

	for (int row = 0; row < m_height; ++row)
	{
		double initX = bottomLeftX;
		double initY = bottomLeftY + step * row;

		auto packaged_task = std::bind(&FractalImageCreator::fillRow, this, row, initX, initY, step);
		tasks.emplace_back(pool.async(packaged_task));
	}

	wait_for_all(tasks);

	return m_buffer.get();
}

void FractalImageCreator::fillRow(int row_n, double initX, double initY, double step)
{
	double Y = initY;
	for (int i = 0; i < m_width; ++i)
	{
		double X = initX + step * i;

		auto [iterations, zR, zI] = Mandelbrot::getIterations(X, Y, nIterations);
		uint32_t value;
		if (iterations < nIterations)
		{
			auto col = colorFrom(iterations, zR, zI);

			value = packRGB( col.R, col.G, col.B );
		}
		else
			value = packRGB(0,0,0);

		*(m_buffer.get() + m_width * row_n + i) = value;
	}
}
