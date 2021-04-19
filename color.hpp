#pragma once

#include "./vec3.hpp"

#include <iostream>

using color = vec3;

inline void write_color( std::ostream& os, const color& v )
{
    // clang-format off
    os << static_cast<int>( 255.99 * v.x() ) << ' '
       << static_cast<int>( 255.99 * v.y() ) << ' '
       << static_cast<int>( 255.99 * v.z() ) << '\n';
    // clang-format on
}
