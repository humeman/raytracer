#include <obj/quad.hpp>

bool Quad::hit(const Ray &r, const Interval &ray_t, HitResult &res) const {
    double denom = normal * r.get_direction();
    if (std::fabs(denom) < EPSILON) return false;

    double t = (d - (normal * r.get_origin())) / denom;
    if (!ray_t.contains(t)) return false;

    Vec3 intersection = r.at(t);
    Vec3 plane_hit = intersection - origin;
    double a = (w * (plane_hit ^ v));
    double b = (w * (u ^ plane_hit));

    if (!Interval::one.contains(a) || !Interval::one.contains(b)) return false;
    
    res.u = a;
    res.v = b;
    res.t = t;
    res.point = intersection;
    res.material = material;
    res.is_front_face = (r.get_direction() * normal) < 0;
    res.normal = res.is_front_face ? normal : -normal;
    return true;
}

AABB Quad::bounding_box() const {
    return box;
}

double Quad::pdf(const Vec3 &start, const Vec3 &direction) const {
    HitResult res;
    if (!hit(Ray(start, direction), Interval(0.001, INFINITY), res))
        return 0;

    auto dist_sq = res.t * res.t * direction.magnitude_squared();
    auto cos = std::fabs((direction * res.normal) / direction.magnitude());
    return dist_sq / (cos * area);
}

Vec3 Quad::random(const Vec3 &start) const {
    auto p = origin + (RAND_DOUBLE() * u) + (RAND_DOUBLE() * v);
    return p - start;
}