#include "ray.hpp"

const Vec3 &Ray::get_origin() const {
    return origin;
}

const Vec3 &Ray::get_direction() const {
    return direction;
}

Vec3 Ray::at(float t) const {
    return origin + direction * t;
}