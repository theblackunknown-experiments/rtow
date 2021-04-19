
#include <charconv>
#include <iostream>

#include <cstring>

int main( int argc, char* argv[] )
{
    int width = 255, height = 255;

    for ( int i = 0; i < argc; ++i )
    {
        if ( strstr( argv[i], "-w" ) || strstr( argv[i], "--width" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, width );
            ++i;
        }
        else if ( strstr( argv[i], "-h" ) || strstr( argv[i], "--height" ) )
        {
            const char*       arg  = argv[i + 1];
            const std::size_t size = std::strlen( arg );
            std::from_chars( arg, arg + size, height );
            ++i;
        }
    }

    std::ios::sync_with_stdio( false );

    std::cerr << "Rendering image " << width << "x" << height << std::endl;

    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    for ( int j = 0; j < height; ++j )
    {
        for ( int i = 0; i < width; ++i )
        {
            auto r = double( i ) / ( width - 1 );
            auto g = double( j ) / ( height - 1 );
            auto b = 0.25;

            int ir = static_cast<int>( 255.99 * r );
            int ig = static_cast<int>( 255.99 * g );
            int ib = static_cast<int>( 255.99 * b );

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}
