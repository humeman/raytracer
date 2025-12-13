#pragma once

#include <math/vec3.hpp>
#include <images/image.hpp>

class Texture {
    public:
        virtual ~Texture() = default;

        virtual Color value(double u, double v, const Vec3 &point) const = 0;
};

class ColorTexture : public Texture {
    private:
        Color albedo;
    
    public:
        ColorTexture(const Color &albedo) : albedo(albedo) {}
        ColorTexture(double r, double g, double b) : albedo(Color(r, g, b)) {}

        Color value(double u, double v, const Vec3 &point) const override;
};

class ImageTexture : public Texture {
    private:
        std::shared_ptr<Image> image;

    public:
        ImageTexture(std::shared_ptr<Image> image) : image(image) {}

        Color value(double u, double v, const Vec3 &point) const override;
};