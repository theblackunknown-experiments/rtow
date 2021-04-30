#pragma once

#include "./vec3.hpp"

#include "./intersection_table.hpp"

namespace rtow {
struct ray;
struct intersection_record;

class intersection_table_sphere: public intersection_table
{
private:
    point center;
    float radius;

public:
    constexpr intersection_table_sphere( const point& p, float r ): center( p ), radius( r )
    {
    }
    bool intersect( const ray& ray, float tmin, float tmax, intersection_record& record ) const override;
};
}  // namespace rtow

#include "./math.hpp"
#include "./vec3.hpp"
#include "./ray.hpp"

#include "./intersection_record.hpp"

namespace rtow {
// r(t) = A + t * b
// (r(t)−C)^2=r^2
// t^2b⋅b+2tb⋅(A−C)+(A−C)⋅(A−C)−r^2=0
inline bool intersection_table_sphere::intersect( const ray& r, float tmin, float tmax, intersection_record& record )
    const
{
    vec3 oc     = r.origin - center;
    auto a      = length_squared( r.direction );  // equivalent to dot( r.direction, r.direction )
    auto half_b = dot( oc, r.direction );
    auto c      = length_squared( oc ) - radius * radius;  // equivalent to dot( oc, oc );

    auto discriminant = half_b * half_b - a * c;
    if ( discriminant < 0 )
        return false;

    auto sqrtd = sqrt( discriminant );
    // search nearest root
    auto root = ( -half_b - sqrtd ) / a;
    if ( root < tmin || tmax < root )
    {
        root = ( -half_b + sqrtd ) / a;
        if ( root < tmin || tmax < root )
            return false;
    }

    record.t      = root;
    record.point  = r.at( root );
    record.normal = ( record.point - center ) / radius;
    return true;
}
}  // namespace rtow
