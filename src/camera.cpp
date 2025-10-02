#include <camera.hpp>
#include <macros.hpp>

Camera::Camera(const CameraParams &params) {
    this->params = params;
    
    this->img = std::make_shared<PPMImage>(params.width, params.height);
    center = Vec3(0.0f, 0.0f, 0.0f);

    float viewport_width = params.viewport_height * ((float) params.width) / params.height;
    Vec3 viewport_u(viewport_width, 0.0f, 0.0f);
    Vec3 viewport_v(0.0f, -params.viewport_height, 0.0f);
    pixel_delta_u = viewport_u / (float) params.width;
    pixel_delta_v = viewport_v / (float) params.height;

    Vec3 upper_left = center - Vec3(0, 0, params.focal_length) - viewport_u / 2.0f - viewport_v / 2.0f;
    pixel00 = upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
}

Color Camera::color(const Scene &scene, const Ray &ray) const {
    HitResult res;
    if (scene.hit(ray, Interval(0, FLOAT_INFINITY), res)) {
        return 0.5f * (res.normal + Color(1.0f, 1.0f, 1.0f));
    }

    Vec3 unit_direction = ray.get_direction().to_unit();
    float a = 0.5f * (unit_direction.b + 1.0f);
    return (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
}

void Camera::render(const Scene &scene) {
    for (int y = 0; y < img->height(); y++) {
        for (int x = 0; x < img->width(); x++) {
            if (params.progress) params.progress(x, y);
            Vec3 pixel_center = pixel00 + ((float) x * pixel_delta_u) + ((float) y * pixel_delta_v);
            Vec3 ray_direction = pixel_center - params.origin;
            Ray r(params.origin, ray_direction);

            img->set(x, y, color(scene, r));
        }
    }
}

std::shared_ptr<Image> Camera::image() const {
    return img;
}