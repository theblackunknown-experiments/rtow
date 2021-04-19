
#include "./vec3.hpp"
#include "./color.hpp"
#include "./ray.hpp"

#include <charconv>
#include <iostream>

#include <cstring>

color pixel_color( const ray& r )
{
    constexpr color a { .5, .7f, 1.f };
    constexpr color b { 1.f, 1.f, 1.f };

    vec3 unit = normalize( r.direction );
    auto t    = 0.5 * ( unit.y() + 1.f );
    return a + t * ( b - a );
}

int main( int argc, char* argv[] )
{
    int   width        = 255;
    float aspect_ratio = 16.f / 9.f;

    for ( int i = 0; i < argc; ++i )
    {
        if ( strstr( argv[i], "-w" ) || strstr( argv[i], "--width" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, width );
            ++i;
        }
        else if ( strstr( argv[i], "-as" ) || strstr( argv[i], "--aspect-ratio" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, aspect_ratio );
            ++i;
        }
    }

    int height = static_cast<int>( width / aspect_ratio );

    constexpr float viewport_height = 2.f;
    float           viewport_width  = aspect_ratio * viewport_height;

    constexpr float focal_length = 1.f;
    constexpr point origin { 0.f, 0.f, 0.f };
    vec3            right { viewport_width, 0.f, 0.f };
    constexpr vec3  up { 0.f, viewport_height, 0.f };

    auto lower_left = origin - right / 2.f - up / 2.f - vec3 { 0.f, 0.f, focal_length };

    std::ios::sync_with_stdio( false );

    std::cerr << "Rendering image " << width << "x" << height << " (aspect ratio: " << aspect_ratio << ")" << std::endl;

    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    for ( int j = 0; j < height; ++j )
    {
        for ( int i = 0; i < width; ++i )
        {
            auto u = float( i ) / ( width - 1 );
            auto v = float( j ) / ( height - 1 );

            ray r { origin, lower_left + u * right + v * up - origin };

            color c = pixel_color( r );

            write_color( std::cout, c );
        }
    }
}
