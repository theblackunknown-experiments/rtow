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

#include "./math.hpp"

namespace rtow {
inline float reflectance( float cos_theta, float etai_over_etat )
{
    // Schlick
    auto r0 = ( 1.f - etai_over_etat ) / ( 1.f + etai_over_etat );
    r0 *= r0;
    return r0 + ( 1.f - r0 ) * pow( 1.f - cos_theta, 5.f );
}

}  // namespace rtow
