#pragma once

#include "./vec3.hpp"
#include "./ray.hpp"
#include "./random.hpp"

namespace rtow {
struct camera
{
    point origin;
    vec3  u, v, w;
    vec3  horizontal;
    vec3  vertical;
    point lower_left;
    float lens_radius;

    ray generate( basic_random_generator& gen, float s, float t ) const
    {
        vec3 lens_origin_delta = lens_radius * random_vec3_in_unit_disk( gen );
        vec3 offset            = u * lens_origin_delta.x() + v * lens_origin_delta.y();

        return ray {
            .origin    = origin + offset,
            .direction = lower_left + s * horizontal + t * vertical - origin - offset,
        };
    }
};

struct fov_camera_parameters
{
    point lookfrom;
    point lookat;
    vec3  vup;

    float vertical_fov;
    float aspect_ratio = 16.f / 9.f;
    float aperture;
    float focus_distance;
};

inline camera create_camera( const fov_camera_parameters& p )
{
    auto theta = degrees_to_radians( p.vertical_fov );
    auto h     = tan( theta / 2.f );

    auto viewport_height = 2.f * h;
    auto viewport_width  = p.aspect_ratio * viewport_height;

    auto w = normalize( p.lookfrom - p.lookat );
    auto u = normalize( cross( p.vup, w ) );
    auto v = cross( w, u );

    auto origin            = p.lookfrom;
    auto horizontal        = p.focus_distance * viewport_width * u;
    auto vertical          = p.focus_distance * viewport_height * v;
    auto lower_left_corner = origin - horizontal / 2.f - vertical / 2.f - p.focus_distance * w;

    return camera { .origin      = origin,
                    .u           = u,
                    .v           = v,
                    .w           = w,
                    .horizontal  = horizontal,
                    .vertical    = vertical,
                    .lower_left  = lower_left_corner,
                    .lens_radius = p.aperture / 2.f };
}
}  // namespace rtow
