#pragma once

#include <cstdint>
#include <memory>
#include <vector>
#include <utility>

using std::unique_ptr;
using std::vector;
using std::pair;

struct RGB {
    uint8_t R;
    uint8_t G;
    uint8_t B;

    static RGB interpolate(RGB& low, RGB& high, double lpos, double hpos, double t);
};

class ColorPalette {
    int size;
    unique_ptr<RGB[]> colorPalete;

public:
    ColorPalette(int size);
    void init(vector<pair<float, RGB>>& vals);
    inline RGB& operator[] (int i) {return colorPalete[i]; }
};

inline constexpr uint32_t packRGB(int r, int g, int b, int a = 255)
{
	return ((a & 0xffu) << 24) | ((r & 0xffu) << 16) | ((g & 0xffu) << 8) | (b & 0xffu);
}
