#include "sphere.hpp"

bool Sphere::hit(const Ray &r, Color &res) const {
    Vec3 oc = center - r.get_origin();
    float a = r.get_direction() * r.get_direction();
    float b = -2.0f * (r.get_direction() * oc);
    float c = (oc * oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant >= 0) {
        res.a = 1.0f;
        res.b = 0.1f;
        res.c = 0.1f;
        return true;
    }
    return false;
}