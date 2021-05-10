
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
#include <fstream>

#include <limits>
#include <memory>
#include <random>
#include <functional>

#include <atomic>
#include <thread>

#include <vector>

#include <cassert>
#include <cstring>
#include <cstdlib>

namespace rtow {

struct light_transport_simple_parameters
{
    int spp;
    int bounces;
};

color ray_color( const intersection_table& intersector, basic_random_generator& gen, const ray& r, int bounces );

void process_scanlines(
    int                                      start,
    int                                      end,
    int                                      height,
    int                                      width,
    const camera&                            camera,
    const intersection_table&                world,
    const light_transport_simple_parameters& p,
    std::vector<color>&                      framebuffer );
}  // namespace rtow

int main( int argc, char* argv[] )
{
    using namespace rtow;

    bool         progress             = false;
    int          height               = 800;
    float        aspect_ratio         = 3.f / 2.f;
    int          spp                  = 500;
    int          bounces              = 50;
    float        vfov                 = 20.f;
    int          output_stream_index  = -1;
    auto         hardware_concurrency = std::thread::hardware_concurrency();
    unsigned int thread_count         = hardware_concurrency;

    std::ios::sync_with_stdio( false );

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
        else if ( strstr( argv[i], "-vfov" ) || strstr( argv[i], "--vertical-field-of-view" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, vfov );
            ++i;
        }
        else if ( strstr( argv[i], "-o" ) || strstr( argv[i], "--output" ) )
        {
            output_stream_index = i + 1;
            ++i;
        }
        else if ( strstr( argv[i], "-j" ) || strstr( argv[i], "--jobs" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );

            int concurrency = 0;
            std::from_chars( arg, arg + size, concurrency );
            ++i;

            if ( concurrency == 0 )
            {
                thread_count = hardware_concurrency;
            }
            else if ( concurrency > 0 )
            {
                thread_count = concurrency;
            }
            else  // if ( concurrency < 0 )
            {
                assert( hardware_concurrency > concurrency );
                if ( hardware_concurrency > concurrency )
                    thread_count = hardware_concurrency - concurrency;
                else
                {
                    std::cerr << "concurrency request is too low (" << concurrency
                              << ") vs actual hardware concurrency (" << hardware_concurrency
                              << "), using a single thread instead" << std::endl;
                    thread_count = 1;
                }
            }
        }
    }

    int width = static_cast<int>( aspect_ratio * height );

    auto camera = create_camera( fov_camera_parameters {
        .lookfrom = { 13.f, 2.f, 3.f },
        .lookat   = { 0.f, 0.f, 0.f },
        .vup      = vec3 { 0.f, 1.f, 0.f },

        .vertical_fov = vfov,
        .aspect_ratio = aspect_ratio,

        .aperture       = 0.1f,
        .focus_distance = 10.f,
    } );

    basic_random_generator generator;

    material_lambertian material_ground( color { 0.5f, 0.5f, 0.5f } );
    material_dielectric material_dielectric( 1.5f );
    material_lambertian material_left( color { 0.4f, 0.2f, 0.1f } );
    material_metal      material_right( color { 0.7f, 0.6f, 0.5f }, 0.f );

    std::vector<material_metal>      metals;
    std::vector<material_lambertian> lambertians;
    metals.reserve( 11 * 11 * 11 * 11 );
    lambertians.reserve( 11 * 11 * 11 * 11 );

    intersection_table_collection intersector_collection;
    intersector_collection.emplace<intersection_table_sphere>( point { 0.f, -1000.f, 0.f }, 1000.f, &material_ground );

    for ( int a = -11; a < 11; ++a )
    {
        for ( int b = -11; b < 11; ++b )
        {
            auto material_choice = random_float( generator );

            vec3 center { a + 0.9f * random_float( generator ), 0.2f, b + 0.9f * random_float( generator ) };

            if ( length( center - point { 4.f, 0.2f, 0.f } ) <= 0.9f )
                continue;

            if ( material_choice < 0.8f )
            {
                auto  albedo   = random_vec3( generator ) * random_vec3( generator );
                auto& material = lambertians.emplace_back( albedo );
                intersector_collection.emplace<intersection_table_sphere>( center, 0.2f, &material );
            }
            else if ( material_choice < 0.95f )
            {
                auto  albedo   = random_vec3( generator, 0.5f, 1.f );
                auto  fuzz     = random_float( generator, 0.f, 0.5f );
                auto& material = metals.emplace_back( albedo, fuzz );
                intersector_collection.emplace<intersection_table_sphere>( center, 0.2f, &material );
            }
            else
            {
                auto albedo = random_vec3( generator, 0.5f, 1.f );
                auto fuzz   = random_float( generator, 0.f, 0.5f );
                intersector_collection.emplace<intersection_table_sphere>( center, 0.2f, &material_dielectric );
            }
        }
    }

    intersector_collection.emplace<intersection_table_sphere>( point { 0.f, 1.f, 0.f }, 1.0f, &material_dielectric );
    intersector_collection.emplace<intersection_table_sphere>( point { -4.f, 1.f, 0.f }, 1.0f, &material_left );
    intersector_collection.emplace<intersection_table_sphere>( point { +4.f, 1.f, 0.f }, 1.0f, &material_right );

    std::ostream* stream = nullptr;
    std::ofstream output_file_stream;
    if ( output_stream_index >= 0 )
    {
        output_file_stream.open( argv[output_stream_index], std::ios_base::out );
        if ( !output_file_stream.is_open() )
            return EXIT_FAILURE;

        stream = &output_file_stream;
    }
    else
    {
        stream = &std::cout;
    }

    std::cerr << "Rendering image " << width << "x" << height << " (aspect ratio: " << aspect_ratio << ") [with "
              << thread_count << " concurrent threads]" << std::endl;

    std::vector<std::thread> pool( thread_count );

    auto chunk_size  = height / thread_count;
    auto chunk_extra = height % thread_count;

    light_transport_simple_parameters p { .spp = spp, .bounces = bounces };

    std::vector<color> framebuffer( height * width );

    for ( auto idx = 0u; idx < thread_count; ++idx )
    {
        auto start = ( idx + 0 ) * chunk_size;
        auto end   = ( idx + 1 ) * chunk_size;
        if ( idx == thread_count - 1 )
        {
            end += chunk_extra;
        }
        assert( start < end );
        pool[idx] = std::thread(
            process_scanlines,
            start,
            end,
            height,
            width,
            camera,
            std::cref( intersector_collection ),
            p,
            std::ref( framebuffer ) );
    }

    for ( std::thread& t : pool )
        t.join();

    pool.clear();

    *stream << "P3\n" << width << ' ' << height << "\n255\n";

    for ( int j = height - 1; j >= 0; --j )
    {
        for ( int i = 0; i < width; ++i )
        {
            auto   index = j * width + i;
            color& c     = framebuffer.at( index );
            write_color( *stream, c, spp );
        }
    }

    std::cerr << "Done." << std::endl;

    if ( output_stream_index >= 0 )
    {
        output_file_stream.close();
    }
    return EXIT_SUCCESS;
}

namespace rtow {

constexpr float kNear = 1e-3f;
constexpr float kFar  = std::numeric_limits<float>::infinity();

void process_scanlines(
    int                                      start,
    int                                      end,
    int                                      height,
    int                                      width,
    const camera&                            camera,
    const intersection_table&                world,
    const light_transport_simple_parameters& p,
    std::vector<color>&                      framebuffer )
{
    basic_random_generator generator;
    for ( int j = start; j < end; ++j )
    {
        for ( int i = 0; i < width; ++i )
        {
            color c { 0.f, 0.f, 0.f };
            for ( int s = 0; s < p.spp; ++s )
            {
                auto u = float( i + random_float( generator ) ) / ( width - 1 );
                auto v = float( j + random_float( generator ) ) / ( height - 1 );

                ray r = camera.generate( generator, u, v );

                c += ray_color( world, generator, r, p.bounces );
            }

            auto idx              = j * width + i;
            framebuffer.at( idx ) = c;
        }
    }
}

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
