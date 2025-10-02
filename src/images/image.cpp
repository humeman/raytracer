#include "image.hpp"
#include "png.hpp"
#include "ppm.hpp"
#include "../macros.hpp"

bool string_ends_with(std::string target, std::string end) {
    return target.size() >= end.size() && target.substr(target.size() - end.size()) == end;
}

Image *Image::read_any(std::string path) {
    if (string_ends_with(path, ".png")) {
        return new PNGImage(path);
    } else if (string_ends_with(path, ".ppm")) {
        return new PPMImage(path);
    } else {
        throw std::runtime_error("unsupported image format: " + path);
    }
}

void Image::write_any(Image &image, std::string path) {
    Image *target;
    bool is_new = false;
    if (string_ends_with(path, ".png")) {
        if (typeid(image) == typeid(PNGImage)) {
            target = &image;
        } else {
            target = new PNGImage(image);
            is_new = true;
        }
    } else if (string_ends_with(path, ".ppm")) {
        if (typeid(image) == typeid(PPMImage)) {
            target = &image;
        } else {
            target = new PPMImage(image);
            is_new = true;
        }
    } else {
        throw std::runtime_error("unsupported image format: " + path);
    }
    target->save(path);
    if (is_new) {
        delete target;
    }
}