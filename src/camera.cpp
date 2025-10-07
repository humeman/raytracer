#include <camera.hpp>
#include <macros.hpp>

#include <iostream>

Camera::Camera(const CameraParams &params) {
    this->params = params;
    
    this->img = std::make_shared<PPMImage>(params.width, params.height);
    center = Vec3(0.0, 0.0, 0.0);

    double viewport_width = params.viewport_height * ((double) params.width) / params.height;
    Vec3 viewport_u(viewport_width, 0.0, 0.0);
    Vec3 viewport_v(0.0, -params.viewport_height, 0.0);
    pixel_delta_u = viewport_u / (double) params.width;
    pixel_delta_v = viewport_v / (double) params.height;

    Vec3 upper_left = center - Vec3(0, 0, params.focal_length) - viewport_u / 2.0 - viewport_v / 2.0;
    pixel00 = upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

Color Camera::color(const Scene &scene, const Ray &ray) const {
    return color(scene, ray, 0);
}

Color Camera::color(const Scene &scene, const Ray &ray, int depth) const {
    if (depth >= params.max_depth) {
        return Color::black;
    }
    double t;
    Vec3 point, normal;
    std::shared_ptr<Object> obj;
    if (scene.hit(ray, Interval(0.001, DOUBLE_INFINITY), obj, t, point, normal)) {
        Ray scattered;
        Color attenuation;
        if (obj->material->scatter(ray, normal, point, attenuation, scattered)) {
            attenuation *= color(scene, scattered, depth + 1);
            return attenuation;
        }
        return Color::black;
    }

    Vec3 unit_direction = ray.get_direction().to_unit();
    double a = 0.5 * (unit_direction.b + 1.0);
    return (1.0 - a) * Color(1.0, 1.0, 1.0) + a * Color(0.5, 0.7, 1.0);
}

Interval ANTIALIAS_OFFSET_INTERVAL(-0.5, 0.5);

Ray Camera::ray(int x, int y) const {
    Vec3 offset(ANTIALIAS_OFFSET_INTERVAL.random(), ANTIALIAS_OFFSET_INTERVAL.random(), 0.0);
    Vec3 sample = pixel00 + ((double) x + offset.a) * pixel_delta_u + ((double) y + offset.b) * pixel_delta_v;
    return Ray(center, sample - center);
}

const Interval INTENSITY_RANGE(0, 0.999);

void Camera::render(const Scene &scene) {
    for (int y = 0; y < img->height(); y++) {
        for (int x = 0; x < img->width(); x++) {
            if (params.progress) params.progress(x, y);
            Color col;
            for (int s = 0; s < params.antialias_samples; s++) {
                Ray r = ray(x, y);

                col += color(scene, r);
            }
            col /= (double) params.antialias_samples;
            col.a = INTENSITY_RANGE.clamp(col.a);
            col.b = INTENSITY_RANGE.clamp(col.b);
            col.c = INTENSITY_RANGE.clamp(col.c);
            col.a = LINEAR_TO_GAMMA(col.a);
            col.b = LINEAR_TO_GAMMA(col.b);
            col.c = LINEAR_TO_GAMMA(col.c);
            img->set(x, y, col);
        }
    }
}

std::shared_ptr<Image> Camera::image() const {
    return img;
}