#pragma once

#include "./vec3.hpp"
#include "./math.hpp"

#include <iostream>

namespace rtow {
using color = vec3;

inline void write_color( std::ostream& os, const color& v, int spp )
{
    auto r = v.x();
    auto g = v.y();
    auto b = v.z();

    auto scale = 1.f / spp;

    // Divide the color by the number of samples and gamma-correct for gamma=2.0.
    r = sqrt( scale * r );
    g = sqrt( scale * g );
    b = sqrt( scale * b );

    // clang-format off
    os << static_cast<int>( 256 * clamp(r, 0.f, 0.999f ) ) << ' '
       << static_cast<int>( 256 * clamp(g, 0.f, 0.999f ) ) << ' '
       << static_cast<int>( 256 * clamp(b, 0.f, 0.999f ) ) << '\n';
    // clang-format on
}
}  // namespace rtow
