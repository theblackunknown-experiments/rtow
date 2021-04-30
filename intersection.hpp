#pragma once

#include <cmath>

#include "./math.hpp"

#include "./ray.hpp"
#include "./vec3.hpp"

namespace rtow {
constexpr float kNoHit = -1.f;

// r(t) = A + t * b
// (r(t)−C)^2=r^2
// t^2b⋅b+2tb⋅(A−C)+(A−C)⋅(A−C)−r^2=0
inline float intersect_sphere( const point& center, float radius, const ray& r )
{
    vec3 oc = r.origin - center;
    auto a  = dot( r.direction, r.direction );
    auto b  = 2.0f * dot( oc, r.direction );
    auto c  = dot( oc, oc ) - radius * radius;

    auto discriminant = b * b - 4 * a * c;
    if ( discriminant < 0 )
    {
        return kNoHit;
    }
    else
    {
        return ( -b - sqrt( discriminant ) ) / ( 2.f * a );
    }
}
}  // namespace rtow
