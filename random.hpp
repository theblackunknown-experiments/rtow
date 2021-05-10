#pragma once

#include <random>

#include "./vec3.hpp"

namespace rtow {

struct basic_random_generator
{
    std::default_random_engine device;
};

float random_float( basic_random_generator& gen, float a = 0.0f, float b = 1.0f )
{
    std::uniform_real_distribution<float> distribution( a, b );
    return distribution( gen.device );
}

vec3 random_vec3( basic_random_generator& gen, float a = 0.0f, float b = 1.0f )
{
    std::uniform_real_distribution<float> distribution( a, b );
    return { distribution( gen.device ), distribution( gen.device ), distribution( gen.device ) };
}

vec3 random_vec3_in_unit_sphere( basic_random_generator& gen )
{
    vec3 p;
    do
    {
        p = random_vec3( gen, -1.f, +1.f );
    } while ( length_squared( p ) >= 1.f );
    return p;
}

vec3 random_vec3_unit_vector( basic_random_generator& gen )
{
    return normalize( random_vec3_in_unit_sphere( gen ) );
}

vec3 random_vec3_in_hemisphere( basic_random_generator& gen, const vec3& normal )
{
    auto in_unit_sphere = random_vec3_in_unit_sphere( gen );
    if ( dot( in_unit_sphere, normal ) > 0.f )
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

vec3 random_vec3_in_unit_disk( basic_random_generator& gen )
{
    vec3 p;

    std::uniform_real_distribution<float> distribution( -1.f, +1.f );
    do
    {
        p = vec3 { distribution( gen.device ), distribution( gen.device ), 0.f };
    } while ( length_squared( p ) >= 1.f );
    return p;
}

}  // namespace rtow
