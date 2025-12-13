#include <camera.hpp>
#include <macros.hpp>

#include <cmath>
#include <iostream>
#include <thread>
#include <mutex>
#include <numbers>

Camera::Camera(const CameraParams &params) {
    this->params = params;
    
    int h = params.height;

    if (params.frac) {
        h = params.height / params.frac_denom;
        if (params.frac_i == params.frac_denom) {
            h = params.height - (params.frac_i - 1) * h;
        }
    }
    this->img = std::make_shared<PPMImage>(params.width, h);
    center = params.look_from;

    double viewport_height = 2 * std::tan(DEG_TO_RAD(params.fov) / 2) * params.focus_distance;
    double viewport_width = viewport_height * ((double) params.width) / params.height;
    Vec3 w = (params.look_from - params.look_at).to_unit();
    Vec3 u = (params.vup ^ w).to_unit();
    Vec3 v = w ^ u;
    Vec3 viewport_u = viewport_width * u;
    Vec3 viewport_v = viewport_height * -v;
    pixel_delta_u = viewport_u / (double) params.width;
    pixel_delta_v = viewport_v / (double) params.height;

    Vec3 upper_left = center - (params.focus_distance * w) - viewport_u / 2.0 - viewport_v / 2.0;
    pixel00 = upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    double defocus_rad = params.focus_distance * std::tan(DEG_TO_RAD(params.defocus_angle / 2));
    defocus_disk_u = u * defocus_rad;
    defocus_disk_v = v * defocus_rad;
}

Color Camera::color(const Scene &scene, const std::vector<std::shared_ptr<Object>> &lights, const Ray &ray, int x, int y) const {
    return color(scene, lights, ray, 0, x, y);
}

Interval HIT_INTERVAL(0.001, DOUBLE_INFINITY);

Color Camera::color(const Scene &scene, const std::vector<std::shared_ptr<Object>> &lights, const Ray &ray, int depth, int x, int y) const {
    if (depth >= params.max_depth) {
        return Color::black;
    }
    HitResult res;
    if (!scene.hit(ray, HIT_INTERVAL, res)) {
        double u = (double) x / params.width;
        double v = (double) y / params.height;
        return params.background->value(u, v, ray.at(1.0));
    }
    ScatterResult scatter_res;
    Color emitted = res.material->emit(res.u, res.v, res.point);
    if (!res.material->scatter(ray, res, scatter_res)) {
        return emitted;
    }

    if (scatter_res.pdf == nullptr) {
        return scatter_res.attenuation % color(scene, lights, scatter_res.scattered, depth + 1, x, y);
    }

    std::shared_ptr<PDF> pdf = std::make_shared<MixPDF>(
        std::make_shared<ObjectListPDF>(lights, res.point),
        scatter_res.pdf
    );

    Ray scattered = Ray(res.point, pdf->generate(), ray.get_time());
    double pdf_val = pdf->value(scattered.get_direction());
    double scattering_pdf = res.material->scattering_pdf(ray, res, scattered);
    Color sample = color(scene, lights, scattered, depth + 1, x, y);
    Color col_scatter = (scatter_res.attenuation * scattering_pdf % sample) / pdf_val;
    return emitted + col_scatter;
}

Interval ANTIALIAS_OFFSET_INTERVAL(-0.5, 0.5);

Ray Camera::ray(int x, int y) const {
    Vec3 offset(ANTIALIAS_OFFSET_INTERVAL.random(), ANTIALIAS_OFFSET_INTERVAL.random(), 0.0);
    Vec3 sample = pixel00 + ((double) x + offset.a) * pixel_delta_u + ((double) y + offset.b) * pixel_delta_v;
    Vec3 origin;
    if (params.defocus_angle > 0.0) {
        Vec3 rand = Vec3::random_in_unit_disk();
        origin = center + rand.a * defocus_disk_u + rand.b * defocus_disk_v;
    } else {
        origin = center;
    }
    return Ray(origin, sample - origin, RAND_DOUBLE());
}

const Interval INTENSITY_RANGE(0, 0.999);

int Camera::next_row() {
    std::lock_guard<std::mutex> guard(queue_lock);
    if (rows.empty()) return -1;
    int r = rows.front();
    last_row = r;
    rows.pop();
    return r;
}

int Camera::current_row() {
    return last_row;
}

void Camera::render(const Scene &scene, std::vector<std::shared_ptr<Object>> &lights) {
    // prepare a queue with the rows to render
    int max_y = params.height;

    if (params.frac) {
        int frac_h = params.height / params.frac_denom;
        frac_y_offset = (params.frac_i - 1) * frac_h;
        if (params.frac_i != params.frac_denom)
            max_y = frac_y_offset + frac_h;
    }

    for (int y = frac_y_offset; y < max_y; y++) {
        rows.push(y);
    }

    // spin up some threads to process these
    active_workers = params.workers;
    for (int i = 0; i < params.workers; i++) {
        std::thread worker(&Camera::threaded_processor, this, std::ref(scene), std::ref(lights));
        worker.detach();
    }

    // wait for finish (and do progress stuff)
    while (true) {
        {
            std::lock_guard<std::mutex> guard(count_lock);
            if (active_workers <= 0) break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (params.progress) params.progress(0, current_row());
    }
}

void Camera::threaded_processor(const Scene &scene, const std::vector<std::shared_ptr<Object>> &lights) {
    int y;
    while ((y = next_row()) >= 0) {
        render_row(scene, lights, y);
    }
    std::lock_guard<std::mutex> guard(count_lock);
    active_workers--;
}

void Camera::render_row(const Scene &scene, const std::vector<std::shared_ptr<Object>> &lights, int y) {
    for (int x = 0; x < img->width(); x++) {
        if (params.progress) params.progress(x, y);
        Ray r;
        Color col = Color(0, 0, 0);
        if (params.adaptive_sampling) {
            Color sum_sq = Color(0, 0, 0);
            int s;
            for (s = 0; s < params.antialias_samples; s++) {
                r = ray(x, y);
                Color c = color(scene, lights, r, x, y);
                col += c;
                sum_sq += c % c;
                
                if (s > 0 && s % params.adaptive_sampling_interval == 0) {
                    double s_double = (double)s;
                    Vec3 mean = col / s_double;
                    Vec3 var_sq = (sum_sq / s_double) - (mean % mean);
                    double n_sqrt = std::sqrt(s);
                    Vec3 confidence(
                        AS_CONFIDENCE_Z * std::sqrt(var_sq.a) / n_sqrt,
                        AS_CONFIDENCE_Z * std::sqrt(var_sq.b) / n_sqrt,
                        AS_CONFIDENCE_Z * std::sqrt(var_sq.c) / n_sqrt
                    );
                    if (
                        confidence.a <= (params.adaptive_sampling_threshold * mean.a)
                        && confidence.b <= (params.adaptive_sampling_threshold * mean.b)
                        && confidence.c <= (params.adaptive_sampling_threshold * mean.c)
                    ) {
                        break;
                    }
                }
            }
            {
                std::lock_guard<std::mutex> guard(as_count_lock);
                as_sample_count += s;
            }
            col /= (s + 1.0);
        } else {
            for (int s = 0; s < params.antialias_samples; s++) {
                r = ray(x, y);
                col += color(scene, lights, r, x, y);
            }

            col /= (double) params.antialias_samples;
        }
        col.a = INTENSITY_RANGE.clamp(col.a);
        col.b = INTENSITY_RANGE.clamp(col.b);
        col.c = INTENSITY_RANGE.clamp(col.c);
        col.a = LINEAR_TO_GAMMA(col.a);
        col.b = LINEAR_TO_GAMMA(col.b);
        col.c = LINEAR_TO_GAMMA(col.c);
        img->set(x, y - frac_y_offset, col);
    }
}

std::shared_ptr<Image> Camera::image() const {
    return img;
}