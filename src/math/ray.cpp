#include <math/ray.hpp>

const Vec3 &Ray::get_origin() const {
    return origin;
}

const Vec3 &Ray::get_direction() const {
    return direction;
}

const double &Ray::get_time() const {
    return time;
}

Vec3 Ray::at(double t) const {
    return origin + direction * t;
}