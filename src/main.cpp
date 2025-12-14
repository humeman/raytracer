#include <images/image.hpp>
#include <images/ppm.hpp>
#include <math/vec3.hpp>
#include <obj/sphere.hpp>
#include <obj/triangle.hpp>
#include <obj/quad.hpp>
#include <obj/material.hpp>
#include <obj/model.hpp>
#include <obj/scene.hpp>
#include <obj/bvh.hpp>
#include <obj/texture.hpp>
#include <obj/cdm.hpp>
#include <macros.hpp>
#include <camera.hpp>
#include <scenes/house.hpp>

#include <stdexcept>
#include <iostream>
#include <cmath>
#include <chrono>
#include <map>
#include <functional>

#define DEFAULT_FILE "out.png"

const std::map<std::string, std::function<void(Scene &, std::vector<std::shared_ptr<Object>> &, CameraParams &)>> SCENES = {
    {"house", house_md}
};

class Flags {
    private:
        bool width_seen = false;
        bool height_seen = false;
        bool file_seen = false;

    public:
        CameraParams params;
        double aspect_ratio = 0.0;
        bool exit = false;
        std::string file = DEFAULT_FILE;

        Flags(int argc, char *argv[]) {
            // preliminary search for -h/--help so if there's an invalid arg, you
            // don't have to clear those all out to find out how to use it
            // this is inefficient of course :)
            for (int i = 1; i < argc; i++) {
                std::string arg = argv[i];
                if (arg == "-h" || arg == "--help") {
                    std::cerr << "Usage: " << argv[0] << " (-x <width>) (-y <height>) (-a <aspect_ratio>) (filename)\n"
                        "  -x/--width:               output image width, defaults to " << params.width << "\n"
                        "  -y/--height:              output image height, defaults to " << params.height << "\n"
                        "  -a/--aspect-ratio:        output aspect ratio, as a double or ratio (16:9, for example)\n"
                        "                            optionally specify a width or a height to apply the aspect ratio to\n"
                        "  -F/--fov:                 field of view in degrees, defaults to " << params.fov << "\n"
                        "  -s/--antialias-samples:   number of antialiasing samples per pixel\n" <<
                        "                             defaults to " << params.antialias_samples << "\n"
                        "  -A/--adaptive-sampling:   enables adaptive sampling\n" <<
                        "  -t/--as-tolerance:        tolerance level (smaller means higher quality) of adaptive sampling\n" <<
                        "                             checks. defaults to " << params.adaptive_sampling_threshold << "\n"
                        "  -d/--max-recursion-depth: maximum recursion depth for one ray\n" <<
                        "                             defaults to " << params.max_depth << "\n" <<
                        "  -w/--workers:             number of worker threads, defaults to " << params.workers << "\n"
                        "  -f/--frac:                renders only a fraction of the image (ie, 1/3, 2/3, and 3/3) for splitting\n"
                        "                             rendering across multiple machines\n"
                        "  -S/--scene:               scene to render, defaults to " << params.scene << "\n" <<
                        "  (filename):               output file, defaults to " DEFAULT_FILE "\n";
                    exit = true;
                    return;
                } 
            }

            for (int i = 1; i < argc; i++) {
                std::string arg = argv[i];
                if (arg == "-x" || arg == "--width") {
                    if (i + 1 >= argc) {
                        throw EXC("-x/--width requires a width value");
                    }
                    params.width = std::stoi(argv[++i]);
                    if (params.width < 1) {
                        throw EXC("-x/--width must be >= 1");
                    }
                    width_seen = true;
                } else if (arg == "-y" || arg == "--height") {
                    if (i + 1 >= argc) {
                        throw EXC("-y/--height requires a height value");
                    }
                    params.height = std::stoi(argv[++i]);
                    if (params.height < 1) {
                        throw EXC("-y/--height must be >= 1");
                    }
                    height_seen = true;
                } else if (arg == "-a" || arg == "--aspect-ratio") {
                    if (i + 1 >= argc) {
                        throw EXC("-a/--aspect-ratio requires a ratio value");
                    }
                    std::string rat_str = argv[++i];
                    try {
                        if (rat_str.find(':') != std::string::npos) {
                            int i = rat_str.find(':');
                            double a = std::stof(rat_str.substr(0, i));
                            double b = std::stof(rat_str.substr(i + 1));
                            if (b == 0.0) {
                                throw EXC("-a/--aspect-ratio can't divide by 0");
                            }
                            aspect_ratio = a / b;
                        } else {
                            aspect_ratio = std::stof(rat_str);
                        }
                    } catch (const std::invalid_argument &e) {
                        throw EXC("-a/--aspect-ratio expects a double or ratio (like 16:9)");
                    }
                    if (aspect_ratio <= 0.0) {
                        throw EXC("-a/--aspect-ratio must be > 0");
                    }
                } else if (arg == "-F" || arg == "--fov") {
                    if (i + 1 >= argc) {
                        throw EXC("-F/--fov requires a degree value");
                    }
                    params.fov = std::stoi(argv[++i]);
                    if (params.fov < 1 || params.fov >= 180) {
                        throw EXC("-F--fov must be >= 1 and < 180");
                    }
                } else if (arg == "-s" || arg == "--antialias-samples") {
                    if (i + 1 >= argc) {
                        throw EXC("-s/--antialias-samples requires a sample count");
                    }
                    params.antialias_samples = std::stoi(argv[++i]);
                } else if (arg == "-A" || arg == "--adaptive-sampling") {
                    params.adaptive_sampling = true;
                } else if (arg == "-t" || arg == "--as-threshold") {
                    if (i + 1 >= argc) {
                        throw EXC("-t/--as-threshold requires a float arg");
                    }
                    params.adaptive_sampling_threshold = std::stod(argv[++i]);
                } else if (arg == "-d" || arg == "--max-recursion-depth") {
                    if (i + 1 >= argc) {
                        throw EXC("-d/--max-recursion-depth requires a depth value");
                    }
                    params.max_depth = std::stoi(argv[++i]);
                } else if (arg == "-w" || arg == "--workers") {
                    if (i + 1 >= argc) {
                        throw EXC("-w/--workers requires a worker count");
                    }
                    params.workers = std::stoi(argv[++i]);
                    if (params.workers <= 0) {
                        throw EXC("-w/--workers must be > 0");
                    }
                } else if (arg == "-f" || arg == "--frac") {
                    if (i + 1 >= argc) {
                        throw EXC("-f/--frac requires a fraction value (like 2/3)");
                    }
                    std::string frac_s = argv[++i];
                    size_t slash = frac_s.find("/");
                    if (slash == std::string::npos) {
                        throw EXC("-f/--frac arg must be a fraction (like 2/3)");
                    }
                    try {
                        params.frac_i = std::stoi(frac_s.substr(0, slash));
                        params.frac_denom = std::stoi(frac_s.substr(slash + 1));
                        params.frac = true;
                    }
                    catch (const std::invalid_argument &e) {
                        throw EXC("-f/--frac fraction parts must be ints");
                    }
                    if (params.frac_i == 0 || params.frac_i > params.frac_denom) {
                        throw EXC("-f/--frac numerator out of bounds (1-indexed)");
                    }
                } else if (arg == "-S" || arg == "--scene") {
                    if (i + 1 >= argc) {
                        throw EXC("-S/--scene requires a scene name");
                    }
                    params.scene = std::string(argv[++i]);
                    if (SCENES.find(params.scene) == SCENES.end()) {
                        std::string all;
                        for (auto &scene : SCENES) {
                            if (!all.empty())
                                all += ", ";
                            all += scene.first;
                        }
                        throw EXC("invalid scene: " + params.scene + "\npick one of: " + all);
                    }
                } else {
                    if (file_seen) {
                        throw EXC("only one output file allowed");
                    }
                    file = arg;
                    file_seen = true;
                }
            }
            if (aspect_ratio != 0.0) {
                if (width_seen && height_seen) {
                    throw EXC("-a/--aspect-ratio requires either -x/--width or -y/--height");
                } else if (height_seen) {
                    params.width = (int) (params.height * aspect_ratio);
                } else {
                    params.height = (int) (params.width / aspect_ratio);
                    if (params.height < 1) {
                        throw EXC("height would be too small for provided width and aspect ratio");
                    }
                }
            }
        }
};

std::string lpad(std::string str, size_t len) {
    std::string res = "";
    size_t count = len < str.length() ? 0 : len - str.length();
    for (size_t i = 0; i < count; i++) {
        res += ' ';
    }
    return res + str;
}

std::string duration_str(std::chrono::seconds duration) {
    uint64_t total = duration.count();
    uint64_t days = total / 86400;
    uint64_t hours = (total % 86400) / 3600;
    uint64_t minutes = (total % 3600) / 60;
    uint64_t seconds = total % 60;

    std::string res;
    if (days > 0) res += lpad(std::to_string(days), 2) + "d ";
    if (days > 0 || hours > 0) res += lpad(std::to_string(hours), 2) + "h ";
    if (days > 0 || hours > 0 || minutes > 0) res += lpad(std::to_string(minutes), 2) + "m ";
    res += lpad(std::to_string(seconds), 2) + "s";
    return res;
}

void progress(int x, int y, CameraParams &params, std::chrono::system_clock::time_point start) {
    if (x != 0) return;
    
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();

    int real_y = y;
    int real_h = params.height;
    if (params.frac) {
        real_h = params.height / params.frac_denom;
        real_y -= (params.frac_i - 1) * real_h;
    }
    
    double prop = (double) (real_y * params.width + x) / (params.width * real_h);

    std::string est = "";
    if (prop > 0.05 && prop < 0.999) {
        std::chrono::nanoseconds elapsed = now - start;
        // it's taken 'elapsed' time to complete 'prop' 
        // calculate the total time
        // (giving up on these types. thanks C++)
        auto total = elapsed / prop;
        auto remaining = total - elapsed;
        est = " (est. " + duration_str(std::chrono::duration_cast<std::chrono::seconds>(remaining)) + ")";
    }

    std::string coord = "y=" + lpad(std::to_string(y), 5);
    std::string percent = lpad(std::to_string((int) (prop * 100.0)), 3);

    CLOG("Rendering (" << coord << "): " << percent << "%" << est);
}



void run(int argc, char *argv[]) {
    Flags flags(argc, argv);
    if (flags.exit) return;

    auto start = std::chrono::system_clock::now();

    Scene scene;
    std::vector<std::shared_ptr<Object>> lights;
    std::cout << "Generating scene..." << std::endl;
    auto scene_generator = SCENES.at(flags.params.scene);
    scene_generator(scene, lights, flags.params);
    int count = scene.size();
    std::cout << "Generating BVH..." << std::endl;
    scene = Scene(std::make_shared<BVHNode>(scene));

    // Give this warning before rendering finishes
    #ifndef PNGPP
    if (flags.file.substr(flags.file.length() - 4) == ".png") {
        std::cerr << "error: install PNG++ and rebuild for PNG support, or use ppms\n";
        return;
    }
    #endif

    std::cout << "Ready!" << std::endl << std::endl;
    std::cout << "Render options:\n"
        << "  Scene: " << flags.params.scene << "\n"
        << "  Output: " << flags.file << "\n"
        << "  Size: " << flags.params.width << "x" << flags.params.height << "\n"
        << "  Antialias samples: " << flags.params.antialias_samples << "\n"
        << "  Adaptive sampling: " << (
            flags.params.adaptive_sampling ? 
                ("yes (t=" + std::to_string(flags.params.adaptive_sampling_threshold) + ")") 
                : "no") << "\n"
        << "  Max recursion depth: " << flags.params.max_depth << "\n"
        << "  Scene: " << count << " objects\n"
        << "  Workers: " << flags.params.workers << "\n"
        << "  Fractional render: " << (flags.params.frac ? "yes" : "no") << "\n";

    if (flags.params.frac) {
        std::cout << "    Part " << flags.params.frac_i << "/" << flags.params.frac_denom << "\n";
    }

    flags.params.progress = [&flags, start](int x, int y) {
        progress(x, y, flags.params, start);
    };
    Camera camera(flags.params);
    camera.render(scene, lights);

    std::shared_ptr<Image> image = camera.image();
    CLOG("Writing " << flags.file << "...");
    Image::write_any(image, flags.file);
    CLOG("File written to " << flags.file << ". Took: " << duration_str(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start)) << "");
    std::cout << std::endl;
    if (flags.params.adaptive_sampling) {
        std::cout << "Average adaptive samples: " << camera.as_sample_count / (flags.params.width * flags.params.height) << " per pixel" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    try {
        run(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << "error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}