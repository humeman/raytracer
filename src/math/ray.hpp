#ifndef RAY_H
#define RAY_H

#include <math/vec3.hpp>

class Ray {
    private:
        Vec3 origin;
        Vec3 direction;
        double time;

    public:
        Ray() : origin(Vec3()), direction(Vec3(1.0, 0.0, 0.0)), time(0) {}
        Ray(const Vec3 &origin, const Vec3 &direction) : origin(origin), direction(direction), time(0) {}
        Ray(const Vec3 &origin, const Vec3 &direction, double time) : origin(origin), direction(direction), time(time) {}

        const double &get_time() const;
        const Vec3 &get_origin() const;
        const Vec3 &get_direction() const;

        Vec3 at(double t) const;
};

#endif