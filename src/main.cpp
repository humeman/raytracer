#include <images/image.hpp>
#include <images/ppm.hpp>
#include <math/vec3.hpp>
#include <obj/sphere.hpp>
#include <obj/material.hpp>
#include <obj/scene.hpp>
#include <macros.hpp>
#include <camera.hpp>

#include <stdexcept>
#include <iostream>
#include <cmath>
#include <chrono>

#define DEFAULT_FILE "out.png"

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
                        "  -x/--width:          output image width, defaults to " << params.width << "\n"
                        "  -y/--height:         output image height, defaults to " << params.height << "\n"
                        "  -a/--aspect-ratio:   output aspect ratio, as a double or ratio (16:9, for example)\n"
                        "                       optionally specify a width or a height to apply the aspect ratio to\n"
                        "  -s/--antialias-samples: number of antialiasing samples per pixel\n" <<
                        "                          defaults to " << params.antialias_samples << "\n"
                        "  -d/--max-recursion-depth: maximum recursion depth for one ray\n" <<
                        "  (filename):          output file, defaults to " DEFAULT_FILE "\n";
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
                } else if (arg == "-s" || arg == "--antialias-samples") {
                    if (i + 1 >= argc) {
                        throw EXC("-s/--antialias-samples requires a sample count");
                    }
                    params.antialias_samples = std::stoi(argv[++i]);
                } else if (arg == "-d" || arg == "--max-recursion-depth") {
                    if (i + 1 >= argc) {
                        throw EXC("-d/--max-recursion-depth requires a depth value");
                    }
                    params.max_depth = std::stoi(argv[++i]);
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

void progress(int x, int y, CameraParams params, std::chrono::system_clock::time_point start) {
    if (x != 0) return;
    
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    
    double prop = (double) (y * params.width + x) / (params.width * params.height);

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
    scene.add(std::make_shared<Sphere>(Vec3(0.0, -100.5, -1.0), 100.0, std::make_shared<Diffuse>(Color(0.8, 0.8, 0.0))));
    scene.add(std::make_shared<Sphere>(Vec3(0.0, 0.0, -1.2), 0.5, std::make_shared<Diffuse>(Color(0.1, 0.2, 0.5))));
    scene.add(std::make_shared<Sphere>(Vec3(-1.0, 0.0, -1.0), 0.5, std::make_shared<Metal>(Color(0.8, 0.8, 0.8))));
    scene.add(std::make_shared<Sphere>(Vec3(1.0, 0.0, -1.0), 0.5, std::make_shared<Metal>(Color(0.8, 0.6, 0.2))));

    flags.params.progress = [flags, start](int x, int y) {
        progress(x, y, flags.params, start);
    };
    Camera camera(flags.params);
    camera.render(scene);

    std::shared_ptr<Image> image = camera.image();
    CLOG("Writing " << flags.file << "...");
    Image::write_any(*image, flags.file);
    CLOG("File written to " << flags.file << ".");
    std::cout << std::endl;
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