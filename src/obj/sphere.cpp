#include "sphere.hpp"
#include <cmath>
#include <optional>

bool Sphere::hit(const Ray &r, const Interval &ray_t, HitResult &result) const {
    Vec3 oc = center - r.get_origin();
    float a = r.get_direction().magnitude_squared();
    float h = r.get_direction() * oc;
    float c = oc.magnitude_squared() - radius * radius;

    float discriminant = h * h - a * c;
    if (discriminant < 0) return false;

    float sqrtd = std::sqrt(discriminant);
    float root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }
    }

    result.t = root;
    result.point = r.at(result.t);
    Vec3 outward_normal = (result.point - center) / radius;
    result.set_face_normal(r, outward_normal);
    return true;
}