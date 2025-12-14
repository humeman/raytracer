#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <obj/scene.hpp>
#include <images/image.hpp>
#include <images/ppm.hpp>
#include <obj/texture.hpp>

#include <queue>
#include <functional>

class CameraParams {
    public:
        int width = 400;
        int height = 200;
        int antialias_samples = 50;
        bool adaptive_sampling = false;
        double adaptive_sampling_threshold = 1.8;
        int adaptive_sampling_interval = 25;
        int max_depth = 50;
        double defocus_angle = 0.6;
        double focus_distance = 5.0;
        double fov = 80;
        std::string scene = "house";

        bool frac = false;
        int frac_i = 0;
        int frac_denom = 0;
        int workers = 4;

        Vec3 origin = Vec3(0.0, 0.0, 0.0);
        Vec3 look_from = Vec3(0.0, 0.0, 0.0);
        Vec3 look_at = Vec3(0.0, 0.0, -1.0);
        Vec3 vup = Vec3(0.0, 1.0, 0.0);

        std::shared_ptr<Texture> background = std::make_shared<ColorTexture>(
            Color::black
        );

        // Called for each pixel
        std::function<void(int, int)> progress = nullptr;
};

class Camera {
    public:
        unsigned long long as_sample_count = 0;
        
        Camera(const CameraParams &params);

        void render(const Scene &scene, std::vector<std::shared_ptr<Object>> &lights);
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
        int frac_y_offset = 0;
        Vec3 center;
        Vec3 pixel00;
        Vec3 pixel_delta_u, pixel_delta_v;
        Vec3 defocus_disk_u, defocus_disk_v;
        std::mutex as_count_lock;

        Color color(const Scene &scene, const std::vector<std::shared_ptr<Object>> &lights, const Ray &ray, int x, int y) const;
        Color color(const Scene &scene, const std::vector<std::shared_ptr<Object>> &lights, const Ray &ray, int depth, int x, int y) const;
        Ray ray(int x, int y, bool rand_offset) const;
        int next_row();
        void render_row(const Scene &scene, const std::vector<std::shared_ptr<Object>> &lights, int y);
        void threaded_processor(const Scene &scene, const std::vector<std::shared_ptr<Object>> &lights);
};

#endif