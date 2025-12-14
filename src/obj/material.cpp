#include <obj/material.hpp>
#include <obj/object.hpp>

#include <numbers>

bool Diffuse::scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const {
    (void) r_in;
    Vec3 direction = res.normal + Vec3::random_unit();
    if (direction.zero()) {
        direction = res.normal;
    }

    scatter_res.scattered = Ray(res.point, direction, r_in.get_time());
    scatter_res.attenuation = texture->value(res.u, res.v, res.point);
    // scatter_res.pdf = std::make_shared<COSPDF>(res.normal);
    return true;
}

double Diffuse::scattering_pdf(const Ray &ray, const HitResult &res, const Ray &scattered) const {
    (void) ray;
    auto cos_t = res.normal * scattered.get_direction().to_unit();
    return cos_t < 0 ? 0 : cos_t / std::numbers::pi;
}

bool Metal::scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const {
    Vec3 reflected = r_in.get_direction().reflect(res.normal).to_unit() + (fuzz * Vec3::random_unit());
    scatter_res.scattered = Ray(res.point, reflected, r_in.get_time());
    scatter_res.attenuation = texture->value(res.u, res.v, res.point);
    return (reflected * res.normal) > 0;
}

bool Dielectric::scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const {
    scatter_res.attenuation = tint;
    double ri = res.is_front_face ? (1.0 / refraction) : refraction;
    Vec3 unit = r_in.get_direction().to_unit();
    double cos_t = std::fmin((-unit * res.normal), 1.0);
    double sin_t = std::sqrt(1.0 - cos_t * cos_t);
    // Schlick's
    double r0 = (1.0 - ri) / (1.0 + ri);
    r0 = r0 * r0;
    double reflectance = r0 + (1.0 - r0) * std::pow((1.0 - cos_t), 5);

    Vec3 direction = ((ri * sin_t > 1.0) || reflectance > RAND_DOUBLE()) ?
        unit.reflect(res.normal) :
        unit.refract(res.normal, ri);
    scatter_res.scattered = Ray(res.point, direction, r_in.get_time());
    return true;
}

bool DiffuseLight::scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const {
    (void) r_in;
    (void) res;
    (void) scatter_res;
    return false;
}

Color DiffuseLight::emit(double u, double v, const Vec3 &point) const {
    return texture->value(u, v, point);
}

bool Isotropic::scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const {
    scatter_res.scattered = Ray(res.point, Vec3::random_unit(), r_in.get_time());
    scatter_res.attenuation = texture->value(res.u, res.v, res.point);
    return true;
}