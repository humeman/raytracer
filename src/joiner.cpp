#include <images/image.hpp>
#include <images/png.hpp>

#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "specify at least 2 files and a destination" << std::endl;
        return 1;
    }

    std::vector<Image *> source;
    for (int i = 1; i < argc - 1; i++) {
        source.push_back(Image::read_any(argv[i]));
    }

    int width = source[0]->width();
    int height = 0;
    for (auto *img : source) {
        if (img->width() != width) {
            std::cerr << "mismatched image widths" << std::endl;
            return 1;
        }
        height += img->height();
    }

    PNGImage *dest = new PNGImage(width, height);

    int y_offset = 0;
    for (auto *img : source) {
        int w = img->width();
        int h = img->height();

        for (int y = 0; y < h; y++) {
            int new_y = y + y_offset;
            for (int x = 0; x < w; x++) {
                dest->set(x, new_y, img->at(x, y));
            }
        }

        y_offset += h;
    }

    Image::write_any(*dest, argv[argc - 1]);

    std::cout << "wrote " << width << "x" << height << " image to " << argv[argc - 1] << std::endl;
    return 0;
}