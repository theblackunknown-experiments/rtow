#pragma once

#include <cmath>

namespace rtow {
inline float sqrt( float v )
{
    return std::sqrt( v );
}

inline float pow( float b, float e )
{
    return std::pow( b, e );
}

inline float cos( float v )
{
    return std::cos( v );
}

inline float sin( float v )
{
    return std::sin( v );
}

inline float tan( float v )
{
    return std::tan( v );
}

inline float abs( float a )
{
    return a < 0.f ? -a : a;
}

inline float min( float a, float b )
{
    return a < b ? a : b;
}

inline float max( float a, float b )
{
    return a > b ? a : b;
}

inline float clamp( float x, float min, float max )
{
    if ( x < min )
        return min;
    if ( x > max )
        return max;
    return x;
}

inline float degrees_to_radians( float degrees )
{
    return degrees * M_PI / 180.0f;
}
}  // namespace rtow
