#pragma once

#include <cmath>

namespace rtow {
inline float sqrt( float v )
{
    return std::sqrt( v );
}

inline float clamp( float x, float min, float max )
{
    if ( x < min )
        return min;
    if ( x > max )
        return max;
    return x;
}
}  // namespace rtow
