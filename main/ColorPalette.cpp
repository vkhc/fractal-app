#include "ColorPalette.h"

#include<algorithm>

using mypair = std::pair<float, RGB>;

 RGB RGB::interpolate(RGB& low, RGB& high, float lpos, float hpos, float t) {
    float i = (t - lpos) / (hpos - lpos);
    RGB result = {uint8_t((high.R - low.R)*i + low.R),
                  uint8_t((high.G - low.G)*i + low.G),
                  uint8_t((high.B - low.B)*i + low.B)};
    return result;
}


ColorPalette::ColorPalette(int size): size(size), colorPalete(new RGB[size]{}) {}

void ColorPalette::init(vector<mypair>& vals) {
    std::sort(vals.begin(), vals.end(), [](mypair& lhs, mypair& rhs) 
                                            { return lhs.first < rhs.first; });
    
    for (int i=0; i<size; ++i) {
        float t = (float)i / size;
        RGB low, high;
        float lpos, hpos;
        low = high = {0, 0, 0};
        lpos = hpos = 0;
        for (int j=1; j<vals.size(); ++j) {
            if (vals[j-1].first <= t && t < vals[j].first) {
                lpos = vals[j-1].first;
                hpos = vals[j].first;
                low = vals[j-1].second;
                high = vals[j].second;
                break;
            }
        }
        colorPalete[i] = RGB::interpolate(low, high, lpos, hpos, t);  
    }
}