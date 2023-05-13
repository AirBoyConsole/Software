#pragma once

#include <iostream>
#include <math.h>

#define SWAP_ENDIAN_16(value)   ( (((value) & 0xFFu) << 8u) | ((value) >> 8u) )

namespace airboy {

class Math {
    public:
        static inline float lerp(float from, float to, float step);
        static inline int lerp(int from, int to, float step);
};

inline float Math::lerp(float from, float to, float step)
{
    return from + (to - from) * step;
}

inline int Math::lerp(int from, int to, float step)
{
    return from + (to - from) * step;
}

}

