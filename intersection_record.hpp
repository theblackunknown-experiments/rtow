#pragma once

#include "./math.hpp"
#include "./vec3.hpp"

namespace rtow {
struct intersection_record
{
    point point;
    vec3  normal;
    float t;
};

}  // namespace rtow
