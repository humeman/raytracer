#include <images/image.hpp>
#include <images/ppm.hpp>
#include <math/vec3.hpp>
#include <obj/sphere.hpp>
#include <obj/scene.hpp>
#include <macros.hpp>
#include <camera.hpp>

#include <stdexcept>
#include <iostream>
#include <cmath>
#include <chrono>

#define DEFAULT_WIDTH 400
#define DEFAULT_HEIGHT 200
#define DEFAULT_FILE "out.png"

class Flags {
    private:
        bool width_seen = false;
        bool height_seen = false;
        bool file_seen = false;

    public:
        int width = DEFAULT_WIDTH;
        int height = DEFAULT_HEIGHT;
        float aspect_ratio = 0.0f;
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
                        "  -x/--width:          output image width, defaults to " STR(DEFAULT_WIDTH) "\n"
                        "  -y/--height:         output image height, defaults to " STR(DEFAULT_HEIGHT) "\n"
                        "  -a/--aspect-ratio:   output aspect ratio, as a float or ratio (16:9, for example)\n"
                        "                       optionally specify a width or a height to apply the aspect ratio to\n"
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
                    width = std::stoi(argv[++i]);
                    if (width < 1) {
                        throw EXC("-x/--width must be >= 1");
                    }
                    width_seen = true;
                } else if (arg == "-y" || arg == "--height") {
                    if (i + 1 >= argc) {
                        throw EXC("-y/--height requires a height value");
                    }
                    height = std::stoi(argv[++i]);
                    if (height < 1) {
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
                            float a = std::stof(rat_str.substr(0, i));
                            float b = std::stof(rat_str.substr(i + 1));
                            if (b == 0.0f) {
                                throw EXC("-a/--aspect-ratio can't divide by 0");
                            }
                            aspect_ratio = a / b;
                        } else {
                            aspect_ratio = std::stof(rat_str);
                        }
                    } catch (const std::invalid_argument &e) {
                        throw EXC("-a/--aspect-ratio expects a float or ratio (like 16:9)");
                    }
                    if (aspect_ratio <= 0.0f) {
                        throw EXC("-a/--aspect-ratio must be > 0");
                    }
                } else {
                    if (file_seen) {
                        throw EXC("only one output file allowed");
                    }
                    file = arg;
                    file_seen = true;
                }
            }
            if (aspect_ratio != 0.0f) {
                if (width_seen && height_seen) {
                    throw EXC("-a/--aspect-ratio requires either -x/--width or -y/--height");
                } else if (height_seen) {
                    width = (int) (height * aspect_ratio);
                } else {
                    height = (int) (width / aspect_ratio);
                    if (height < 1) {
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
    
    float prop = (float) (y * params.width + x) / (params.width * params.height);

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
    std::string percent = lpad(std::to_string((int) (prop * 100.0f)), 3);

    CLOG("Rendering (" << coord << "): " << percent << "%" << est);
}

void run(int argc, char *argv[]) {
    Flags flags(argc, argv);
    if (flags.exit) return;

    auto start = std::chrono::system_clock::now();

    Scene scene;
    scene.add(std::make_shared<Sphere>(Vec3(0.0f, 0.0f, -1.0f), 0.5f));
    scene.add(std::make_shared<Sphere>(Vec3(0.0f, -100.5f, -1.0f), 100.0f));

    CameraParams params;
    params.width = flags.width;
    params.height = flags.height;
    params.progress = [params, start](int x, int y) {
        progress(x, y, params, start);
    };
    Camera camera(params);
    camera.render(scene);

    std::shared_ptr<Image> image = camera.image();
    CLOG("Writing " << flags.file << "...");
    Image::write_any(*image, flags.file);
    CLOG("Done!");
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