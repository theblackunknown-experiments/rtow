#pragma once

#include "./vec3.hpp"
#include "./ray.hpp"

namespace rtow {
struct camera
{
    point origin;
    vec3  right;
    vec3  up;
    point lower_left;

    ray generate( float u, float v ) const
    {
        return ray {
            .origin    = origin,
            .direction = lower_left + u * right + v * up - origin,
        };
    }
};

struct simple_camera_parameters
{
    float aspect_ratio    = 16.f / 9.f;
    float viewport_height = 2.f;
    float focal_length    = 1.f;
};

inline camera create_camera( const simple_camera_parameters& p )
{
    auto viewport_width = p.aspect_ratio * p.viewport_height;

    vec3 origin { 0.f, 0.f, 0.f };
    vec3 right { viewport_width, 0.f, 0.f };
    vec3 up { 0.f, p.viewport_height, 0.f };
    return camera { .origin     = origin,
                    .right      = right,
                    .up         = up,
                    .lower_left = origin - right / 2.f - up / 2.f - vec3 { 0.f, 0.f, p.focal_length } };
}

struct fov_camera_parameters
{
    point lookfrom;
    point lookat;
    vec3  vup;

    float vertical_fov;
    float aspect_ratio = 16.f / 9.f;
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
    auto horizontal        = viewport_width * u;
    auto vertical          = viewport_height * v;
    auto lower_left_corner = origin - horizontal / 2.f - vertical / 2.f - w;

    return camera { .origin = origin, .right = horizontal, .up = vertical, .lower_left = lower_left_corner };
}
}  // namespace rtow
