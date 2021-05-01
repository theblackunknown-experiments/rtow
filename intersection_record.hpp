#pragma once

#include "./math.hpp"
#include "./vec3.hpp"

namespace rtow {

class material;

struct intersection_record
{
    point     point;
    vec3      normal;
    float     t;
    bool      front_face;
    material* material;

    void update_normal( const ray& r, const vec3& outward_normal )
    {
        front_face = dot( r.direction, outward_normal ) < 0.f;
        normal     = front_face ? outward_normal : -outward_normal;
    }
};

}  // namespace rtow
