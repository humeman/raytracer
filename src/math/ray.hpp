#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

class Ray {
    private:
        Vec3 origin;
        Vec3 direction;

    public:
        Ray() : origin(Vec3()), direction(Vec3(1.0f, 0.0f, 0.0f)) {}
        Ray(const Vec3 &origin, const Vec3 &direction) : origin(origin), direction(direction) {}

        const Vec3 &get_origin() const;
        const Vec3 &get_direction() const;

        Vec3 at(float t) const {
            return origin + direction * t;
        }
};

#endif