#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <obj/scene.hpp>
#include <images/image.hpp>
#include <images/ppm.hpp>

#include <queue>
#include <functional>

class CameraParams {
    public:
        int width = 400;
        int height = 200;
        int antialias_samples = 10;
        int max_depth = 50;
        int workers = 4;

        bool frac = false;
        int frac_i = 0;
        int frac_denom = 0;

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
        int current_row();
        
    private:
        std::shared_ptr<Image> img;
        std::mutex queue_lock;
        std::queue<int> rows;
        std::mutex count_lock;
        int last_row = 0;
        int active_workers = 0;
        CameraParams params;

        Vec3 center;
        Vec3 pixel00;
        Vec3 pixel_delta_u;
        Vec3 pixel_delta_v;

        Color color(const Scene &scene, const Ray &ray) const;
        Color color(const Scene &scene, const Ray &ray, int depth) const;
        Ray ray(int x, int y) const;
        int next_row();
        void render_row(const Scene &scene, int y);
        void threaded_processor(const Scene &scene);
};

#endif