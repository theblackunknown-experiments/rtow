#pragma once

#include "./material.hpp"

namespace rtow {

class material_metal: public material
{
private:
    color albedo;

public:
    material_metal( const color& a ): albedo( a )
    {
    }
    bool scatter(
        const ray&                 r_in,
        const intersection_record& record,
        basic_random_generator&    generator,
        color&                     attenuation,
        ray&                       r_out ) const override;
};

}  // namespace rtow

#include "./ray.hpp"
#include "./random.hpp"

namespace rtow {

bool material_metal::scatter(
    const ray&                 r_in,
    const intersection_record& record,
    basic_random_generator&    generator,
    color&                     attenuation,
    ray&                       r_out ) const
{
    auto scattered = reflect( normalize( r_in.direction ), record.normal );
    r_out          = ray { record.point, scattered };
    attenuation    = albedo;
    return dot( scattered, record.normal ) > 0.f;
}

}  // namespace rtow
