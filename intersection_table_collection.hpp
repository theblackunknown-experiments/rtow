#pragma once

#include <vector>
#include <utility>

#include "./intersection_table.hpp"

namespace rtow {
struct ray;
struct intersection_record;

class intersection_table_collection: public intersection_table
{
private:
    std::vector<std::unique_ptr<intersection_table>> collection;

public:
    template <typename T, typename... Args>
    void emplace( Args&&... args )
    {
        collection.emplace_back( std::make_unique<T>( std::forward<Args>( args )... ) );
    }

    template <typename T>
    void emplace( T&& t )
    {
        collection.emplace_back( std::make_unique<T>( std::move( t ) ) );
    }

    void clear();

    bool intersect( const ray& ray, float tmin, float tmax, intersection_record& record ) const override;
};
}  // namespace rtow

#include "./intersection_record.hpp"

namespace rtow {

void intersection_table_collection::clear()
{
    collection.clear();
}

inline bool
intersection_table_collection::intersect( const ray& r, float tmin, float tmax, intersection_record& record ) const
{
    intersection_record local_record;
    float               closest_hit     = tmax;
    bool                any_intersected = false;

    for ( auto&& intersector : collection )
    {
        if ( intersector->intersect( r, tmin, closest_hit, local_record ) )
        {
            any_intersected = true;
            closest_hit     = local_record.t;
            record          = local_record;
        }
    }
    return any_intersected;
}
}  // namespace rtow
