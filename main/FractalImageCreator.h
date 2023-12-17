#pragma once

#include <memory>

#include "thread_pool.h"
#include "ColorPalette.h"

class FractalImageCreator
{
public:
	FractalImageCreator(int width, int height);

	/* */
	uint32_t* createImageRaw(double cX, double cY, double radius);

	void fillImage(uint32_t* buffer, int width, int height, double initX, double initY, double radius);

private:
	RGB colorFrom(int iterations, double zR, double zI);

private:
	int m_width;
	int m_height;
	std::unique_ptr<uint32_t[]> m_buffer;
    int nIterations = 1000;
    ColorPalette palette;
	TPool& pool;
};

