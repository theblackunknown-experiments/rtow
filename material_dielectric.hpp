#pragma once

#include "./material.hpp"

namespace rtow {

class material_dielectric: public material
{
private:
    float eta;

public:
    material_dielectric( float eta ): eta( eta )
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
#include "./math.hpp"
#include "./random.hpp"

namespace rtow {

bool material_dielectric::scatter(
    const ray&                 r_in,
    const intersection_record& record,
    basic_random_generator&    generator,
    color&                     attenuation,
    ray&                       r_out ) const
{
    constexpr float kEtaAir = 1.f;

    attenuation = color { 1.f, 1.f, 1.f };
    float ratio = record.front_face ? kEtaAir / eta : eta / kEtaAir;

    auto unit_direction = normalize( r_in.direction );
    auto cos_theta      = min( dot( -unit_direction, record.normal ), 1.f );
    auto sin_theta      = sqrt( 1.f - cos_theta * cos_theta );

    std::uniform_real_distribution<float> distribution;

    if ( ( ratio * sin_theta > 1.f ) || reflectance( cos_theta, ratio ) > distribution( generator.device ) )
    {
        auto scattered = reflect( unit_direction, record.normal );
        r_out          = ray { record.point, scattered };
    }
    else
    {
        auto refracted = refract( unit_direction, record.normal, ratio, cos_theta );
        r_out          = ray { record.point, refracted };
    }

    return true;
}

}  // namespace rtow
