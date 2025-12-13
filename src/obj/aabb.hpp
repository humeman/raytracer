#pragma once

#include <math/interval.hpp>
#include <math/ray.hpp>
#include <math/vec3.hpp>

class AABB {
    public:
        Interval x, y, z;

        AABB() {}
        AABB(const Interval &x, const Interval &y, const Interval &z)
            : x(x), y(y), z(z) {
            pad();
        }

        AABB(const Vec3 &a, const Vec3 &b)
            : x((a.a <= b.a) ? Interval(a.a, b.a) : Interval(b.a, a.a)),
              y((a.b <= b.b) ? Interval(a.b, b.b) : Interval(b.b, a.b)),
              z((a.c <= b.c) ? Interval(a.c, b.c) : Interval(b.c, a.c)) {
                pad();
        }

        AABB(const AABB &a, const AABB &b);

        const Interval &axis_interval(Direction &dir) const;
        bool hit(const Ray &r, Interval ray_t) const;
        Direction longest_axis() const;

        static const AABB empty, universe;
    
    private:
        void pad();
};