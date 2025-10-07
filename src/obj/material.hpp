#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <math/ray.hpp>

class Material {
    public:
        virtual ~Material() = default;
        
        virtual bool scatter(const Ray &r_in, const Vec3 &normal, const Vec3 &point, Color &attenuation, Ray &scattered) const = 0;    
};

class Diffuse : public Material {
    private:
        Color albedo;

    public:
        Diffuse(const Color albedo) : albedo(albedo) {}

        bool scatter(const Ray &r_in, const Vec3 &normal, const Vec3 &point, Color &attenuation, Ray &scattered) const override;
};

class Metal : public Material {
    private:
        Color albedo;

    public:
        Metal(const Color albedo) : albedo(albedo) {}

        bool scatter(const Ray &r_in, const Vec3 &normal, const Vec3 &point, Color &attenuation, Ray &scattered) const override;
};

#endif