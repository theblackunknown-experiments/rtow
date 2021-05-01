#pragma once

namespace rtow {

struct ray;
struct vec3;
struct intersection_record;
struct basic_random_generator;

using color = vec3;

class material
{
public:
    virtual bool scatter(
        const ray&                 r_in,
        const intersection_record& record,
        basic_random_generator&    generator,
        color&                     attenuation,
        ray&                       r_out ) const = 0;
};

}  // namespace rtow
