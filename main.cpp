
#include "./vec3.hpp"
#include "./color.hpp"
#include "./ray.hpp"

#include "./random.hpp"

#include "./camera.hpp"

#include "./intersection_record.hpp"
#include "./intersection_table_sphere.hpp"
#include "./intersection_table_collection.hpp"

#include "./material_dielectric.hpp"
#include "./material_lambertian.hpp"
#include "./material_metal.hpp"

#include <charconv>
#include <iostream>

#include <limits>
#include <memory>
#include <random>

#include <cstring>

namespace rtow {

constexpr float kNear = 1e-3f;
constexpr float kFar  = std::numeric_limits<float>::infinity();

color ray_color( const intersection_table& intersector, basic_random_generator& gen, const ray& r, int bounces )

{
    if ( bounces < 0 )
        return { 0.f, 0.f, 0.f };

    intersection_record record;
    if ( intersector.intersect( r, kNear, kFar, record ) )
    {
        ray   scattered;
        color attenuation;
        if ( record.material->scatter( r, record, gen, attenuation, scattered ) )
            return attenuation * ray_color( intersector, gen, scattered, bounces - 1 );
        else
            return { 0.f, 0.f, 0.f };
    }

    constexpr color a { 1.f, 1.f, 1.f };
    constexpr color b { .5f, .7f, 1.f };

    vec3 unit = normalize( r.direction );

    auto t = 0.5 * ( unit.y() + 1.f );
    return a + t * ( b - a );
}
}  // namespace rtow

int main( int argc, char* argv[] )
{
    using namespace rtow;

    bool  progress     = false;
    int   height       = 480;
    float aspect_ratio = 4.f / 3.f;
    int   spp          = 100;
    int   bounces      = 50;

    for ( int i = 0; i < argc; ++i )
    {
        if ( strstr( argv[i], "-h" ) || strstr( argv[i], "--height" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, height );
            ++i;
        }
        else if ( strstr( argv[i], "-as" ) || strstr( argv[i], "--aspect-ratio" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, aspect_ratio );
            ++i;
        }
        else if ( strstr( argv[i], "-spp" ) || strstr( argv[i], "--sample-per-pixel" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, spp );
            ++i;
        }
        else if ( strstr( argv[i], "--bounces" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, bounces );
            ++i;
        }
        else if ( strstr( argv[i], "--progress" ) )
        {
            progress = true;
        }
    }

    int width = static_cast<int>( aspect_ratio * height );

    auto camera = create_camera( simple_camera_parameters {
        .aspect_ratio    = aspect_ratio,
        .viewport_height = 2.f,
        .focal_length    = 1.f,
    } );

    std::ios::sync_with_stdio( false );

    basic_random_generator                random_generator;
    std::uniform_real_distribution<float> jitter( 0.f, 1.f );

    material_metal      material_metal_left( color { 0.8f, 0.8f, 0.8f }, 0.3f );
    material_metal      material_metal_right( color { 0.8f, 0.6f, 0.2f }, 1.0f );
    material_dielectric material_glass( 1.5f );
    material_lambertian material_ground( color { 0.8f, 0.8f, 0.0f } );
    material_lambertian material_center( color { 0.7f, 0.3f, 0.3f } );

    intersection_table_collection intersector_collection;
    intersector_collection.emplace<intersection_table_sphere>( point { 0.f, -100.5f, -1.f }, 100.f, &material_ground );
    intersector_collection.emplace<intersection_table_sphere>( point { 0.f, 0.f, -1.f }, 0.5f, &material_center );
    intersector_collection.emplace<intersection_table_sphere>( point { -1.f, 0.f, -1.f }, 0.5f, &material_glass );
    intersector_collection.emplace<intersection_table_sphere>( point { -1.f, 0.f, -1.f }, -0.4f, &material_glass );
    intersector_collection.emplace<intersection_table_sphere>( point { 1.f, 0.f, -1.f }, 0.5f, &material_metal_right );

    std::cerr << "Rendering image " << width << "x" << height << " (aspect ratio: " << aspect_ratio << ")" << std::endl;

    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    for ( int j = height - 1; j >= 0; --j )
    {
        if ( progress )
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for ( int i = 0; i < width; ++i )
        {
            color c { 0.f, 0.f, 0.f };
            for ( int s = 0; s < spp; ++s )
            {
                auto u = float( i + jitter( random_generator.device ) ) / ( width - 1 );
                auto v = float( j + jitter( random_generator.device ) ) / ( height - 1 );

                ray r = camera.generate( u, v );

                c += ray_color( intersector_collection, random_generator, r, bounces );
            }

            write_color( std::cout, c, spp );
        }
    }

    if ( progress )
        std::cerr << std::endl;

    std::cerr << "Done." << std::endl;
}
