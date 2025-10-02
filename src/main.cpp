#include "images/image.hpp"
#include "images/ppm.hpp"
#include "macros.hpp"

#include <stdexcept>
#include <iostream>

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

void run(int argc, char *argv[]) {
    Flags flags(argc, argv);
    if (flags.exit) return;
    PPMImage *img = new PPMImage(flags.width, flags.height);
    CLOG("Writing " << flags.file << "...");
    Image::write_any(*img, flags.file);
    CLOG("Done!");
    std::cout << std::endl;
    delete img;
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