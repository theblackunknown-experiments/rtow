
#include "./vec3.hpp"
#include "./color.hpp"
#include "./ray.hpp"
#include "./intersection_record.hpp"
#include "./intersection_table_sphere.hpp"

#include <charconv>
#include <iostream>

#include <cstring>

namespace rtow {

constexpr intersection_table_sphere kSphere { { 0.f, 0.f, -1.f }, 0.5f };
constexpr float                     kNear = 1e-3f;
constexpr float                     kFar  = 1e3f;

color pixel_color( const ray& r )
{
    constexpr vec3 kSphereCenter { 0.f, 0.f, -1.f };

    intersection_record record;
    if ( kSphere.intersect( r, kNear, kFar, record ) )
    {
        return record.front_face ? 0.5f * color { record.normal + 1.f } : color { 0.f, 0.f, 0.f };
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
        else if ( strstr( argv[i], "--progress" ) )
        {
            progress = true;
        }
    }

    int width = static_cast<int>( aspect_ratio * height );

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

    for ( int j = height - 1; j >= 0; --j )
    {
        if ( progress )
            std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
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
