#include "sphere.hpp"
#include <cmath>
#include <optional>
#include <iostream>

bool Sphere::hit(const Ray &r, const Interval &ray_t, HitResult &res) const {
    Vec3 center_t = center.at(r.get_time());
    Vec3 oc = center_t - r.get_origin();
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

    res.t = root;
    res.point = r.at(res.t);
    Vec3 outward_normal = (res.point - center_t) / radius;
    res.is_front_face = (r.get_direction() * outward_normal) < 0;
    res.normal = res.is_front_face ? outward_normal : -1 * outward_normal;
    res.material = material;
    uv(outward_normal, res.u, res.v);
    return true;
}

AABB Sphere::bounding_box() const {
    return box;
}

void Sphere::uv(const Vec3 &p, double &u, double &v) {
    double theta = std::acos(-p.b);
    double phi = std::atan2(-p.c, p.a) + std::numbers::pi;
    u = phi / (2 * std::numbers::pi);
    v = theta / std::numbers::pi;
}