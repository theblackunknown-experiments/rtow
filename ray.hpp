#pragma once

#include "./vec3.hpp"

#include <iostream>

struct ray
{
    point origin;
    vec3  direction;

    point at( float t ) const
    {
        return origin + t * direction;
    }
};
