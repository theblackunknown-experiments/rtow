#pragma once

namespace rtow {
struct ray;
struct intersection_record;

class intersection_table
{
public:
    virtual bool intersect( const ray& ray, float tmin, float tmax, intersection_record& record ) const = 0;
};
}  // namespace rtow
