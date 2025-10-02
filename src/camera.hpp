#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <obj/scene.hpp>
#include <images/image.hpp>
#include <images/ppm.hpp>

#include <functional>

class CameraParams {
    public:
        int width;
        int height;

        float focal_length = 1.0f;
        float viewport_height = 2.0f;

        Vec3 origin = Vec3(0.0f, 0.0f, 0.0f);

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
};

#endif