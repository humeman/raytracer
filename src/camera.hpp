#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <obj/scene.hpp>
#include <images/image.hpp>
#include <images/ppm.hpp>

#include <functional>

class CameraParams {
    public:
        int width = 400;
        int height = 200;
        int antialias_samples = 10;
        int max_depth = 50;

        double focal_length = 1.0;
        double viewport_height = 2.0;

        Vec3 origin = Vec3(0.0, 0.0, 0.0);

        // Called for each pixel
        std::function<void(int, int)> progress = nullptr;
};

class Camera {
    public:
        Camera(const CameraParams &params);

        void render(const Scene &scene);
        std::shared_ptr<Image> image() const;

    private:
        std::shared_ptr<Image> img;
        CameraParams params;

        Vec3 center;
        Vec3 pixel00;
        Vec3 pixel_delta_u;
        Vec3 pixel_delta_v;

        Color color(const Scene &scene, const Ray &ray) const;
        Color color(const Scene &scene, const Ray &ray, int depth) const;
        Ray ray(int x, int y) const;
};

#endif