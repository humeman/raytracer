#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <math/ray.hpp>
#include <math/pdf.hpp>
#include <obj/texture.hpp>
#include <obj/object.hpp>
#include <obj/hit.hpp>

#include <memory>

class PDF;

class ScatterResult {
    public:
        Color attenuation; 
        std::shared_ptr<PDF> pdf = nullptr;
        Ray scattered;
};

class Material {
    public:
        virtual ~Material() = default;

        virtual bool scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const = 0;    
        virtual Color emit(double u, double v, const Vec3 &point) const {
            (void) u;
            (void) v;
            (void) point;
            return Color::black;
        }
        virtual double scattering_pdf(const Ray &ray, const HitResult &res, const Ray &scattered) const {
            (void) ray; (void) res; (void) scattered;
            return 0;
        }
};

class Diffuse : public Material {
    private:
        std::shared_ptr<Texture> texture;

    public:
        Diffuse(std::shared_ptr<Texture> texture) : texture(texture) {}

        bool scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const override;

        double scattering_pdf(const Ray &ray, const HitResult &res, const Ray &scattered) const override;
};

class Metal : public Material {
    private:
    std::shared_ptr<Texture> texture;
        double fuzz;

    public:
        Metal(std::shared_ptr<Texture> texture, double fuzz) : texture(texture), fuzz(fuzz) {}

        bool scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const override;
};

class Dielectric : public Material {
    private:
        double refraction;
        Color tint;

    public:
        Dielectric(double refraction, Color tint = Color::white) 
            : refraction(refraction), tint(tint) {}

        bool scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const override;
};

class DiffuseLight : public Material {
    private:
        std::shared_ptr<Texture> texture;

    public:
        DiffuseLight(std::shared_ptr<Texture> texture) : texture(texture) {}

        bool scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const override;
        Color emit(double u, double v, const Vec3 &point) const override;
};

class Isotropic : public Material {
    private:
        std::shared_ptr<Texture> texture;

    public:
        Isotropic(std::shared_ptr<Texture> texture) : texture(texture) {}

        bool scatter(const Ray &r_in, const HitResult &res, ScatterResult &scatter_res) const override;
};

#endif