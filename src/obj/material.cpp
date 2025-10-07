#include <obj/material.hpp>

bool Diffuse::scatter(const Ray &r_in, const Vec3 &normal, const Vec3 &point, Color &attenuation, Ray &scattered) const {
    (void) r_in;
    Vec3 direction = normal + Vec3::random_unit();
    if (direction.zero()) {
        direction = normal;
    }

    scattered = Ray(point, direction);
    attenuation = albedo;
    return true;
}

bool Metal::scatter(const Ray &r_in, const Vec3 &normal, const Vec3 &point, Color &attenuation, Ray &scattered) const {
    Vec3 reflected = REFLECT(r_in.get_direction(), normal);
    scattered = Ray(point, reflected);
    attenuation = albedo;
    return true;
}