#pragma once

#include "./material.hpp"

namespace rtow {

class material_lambertian: public material
{
private:
    color albedo;

public:
    material_lambertian( const color& a ): albedo( a )
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

bool material_lambertian::scatter(
    const ray&                 r_in,
    const intersection_record& record,
    basic_random_generator&    generator,
    color&                     attenuation,
    ray&                       r_out ) const
{
    auto scattered = random_vec3_in_hemisphere( generator, record.normal );
    // auto scattered = record.normal + random_vec3_unit_vector( generator );

    // catch degenerated case, arbitrary choose normal
    if ( near_zero( scattered ) )
        scattered = record.normal;

    r_out       = ray { record.point, scattered };
    attenuation = albedo;
    return true;
}

}  // namespace rtow
