#include <obj/triangle.hpp>

bool Triangle::hit(const Ray &r, const Interval &ray_t, HitResult &res) const {
    // Implementation of:
    // "Fast, Minimum Storage Ray/Triangle Intersection"
    // Tomas Möller and Ben Trumbore
    Vec3 edge1 = b - a;
    Vec3 edge2 = c - a;

    Vec3 p = r.get_direction() ^ edge2;
    double determinant = edge1 * p;

    if (Interval::epsilon.contains(determinant)) {
        return false;
    }
    double inverse_determinant = 1.0 / determinant;

    Vec3 t = r.get_origin() - a;
    double u = (t * p) * inverse_determinant;
    if (u < 0.0 || u > 1.0) {
        return false;
    }
    Vec3 q = t ^ edge1;
    double v = (r.get_direction() * q) * inverse_determinant;
    if (v < 0.0 || u + v > 1.0) {
        return false;
    }
    double intersect_t = (edge2 * q) * inverse_determinant;
    if (!ray_t.surrounds(intersect_t)) {
        return false;
    }
    res.t = intersect_t;
    res.point = r.at(res.t);

    res.u = (1.0 - u - v) * uv_a.a + u * uv_b.a + v * uv_c.a;
    res.v = (1.0 - u - v) * uv_a.b + u * uv_b.b + v * uv_c.b;

    if (r.get_direction() * normal < 0) {
        res.is_front_face = true;
        res.normal = normal;
    } else {
        res.is_front_face = false;
        res.normal = -normal;
    }
    res.material = material;
    return true;
}

AABB Triangle::bounding_box() const {
    return box;
}