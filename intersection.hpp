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
    vec3 oc     = r.origin - center;
    auto a      = length_squared( r.direction );  // equivalent to dot( r.direction, r.direction )
    auto half_b = dot( oc, r.direction );
    auto c      = length_squared( oc ) - radius * radius;  // equivalent to dot( oc, oc );

    auto discriminant = half_b * half_b - a * c;
    if ( discriminant < 0 )
    {
        return kNoHit;
    }
    else
    {
        return ( -half_b - sqrt( discriminant ) ) / a;
    }
}
}  // namespace rtow
