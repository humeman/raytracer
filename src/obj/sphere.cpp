#include "sphere.hpp"
#include <cmath>
#include <optional>

bool Sphere::hit(const Ray &r, const Interval &ray_t, double &res_t, Vec3 &res_point, Vec3 &res_normal) const {
    Vec3 oc = center - r.get_origin();
    double a = r.get_direction().magnitude_squared();
    double h = r.get_direction() * oc;
    double c = oc.magnitude_squared() - radius * radius;

    double discriminant = h * h - a * c;
    if (discriminant < 0) return false;

    double sqrtd = std::sqrt(discriminant);
    double root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            return false;
        }
    }

    res_t = root;
    res_point = r.at(res_t);
    Vec3 outward_normal = (res_point - center) / radius;
    bool front_face = (r.get_direction() * outward_normal) < 0;
    res_normal = front_face ? outward_normal : -1 * outward_normal;
    return true;
}