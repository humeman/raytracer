#include <obj/cdm.hpp>
#include <cmath>
#include <optional>
#include <iostream>


bool CDM::hit(const Ray &r, const Interval &ray_t, HitResult &res) const {
    HitResult res_a, res_b;
    if (!boundary->hit(r, Interval::universe, res_a))
        return false;
    if (!boundary->hit(r, Interval(res_a.t + 0.0001, INFINITY), res_b))
        return false;
    if (res_a.t < ray_t.min) res_a.t = ray_t.min;
    if (res_b.t > ray_t.max) res_b.t = ray_t.max;

    if (res_a.t >= res_b.t) return false;
    res_a.t = std::max(0.0, res_a.t);

    double len = r.get_direction().magnitude();
    double dist = (res_b.t - res_a.t) * len;
    double hit_dist = (-1 / density) * std::log(RAND_DOUBLE());
    if (hit_dist > dist) return false;

    res.t = res_a.t + hit_dist / len;
    res.point = r.at(res.t);
    res.normal = Vec3(1, 0, 0);
    res.is_front_face = true;
    res.material = material;
    
    return true;
}

AABB CDM::bounding_box() const {
    return boundary->bounding_box();
}
