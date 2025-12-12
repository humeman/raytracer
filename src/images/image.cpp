#include <images/image.hpp>
#include <images/png.hpp>
#include <images/ppm.hpp>
#include <macros.hpp>

bool string_ends_with(std::string target, std::string end) {
    return target.size() >= end.size() && target.substr(target.size() - end.size()) == end;
}

std::shared_ptr<Image> Image::read_any(std::string path) {
    if (string_ends_with(path, ".png")) {
        return std::make_shared<PNGImage>(path);
    } else if (string_ends_with(path, ".ppm")) {
        return std::make_shared<PPMImage>(path);
    } else {
        throw std::runtime_error("unsupported image format: " + path);
    }
}

void Image::write_any(std::shared_ptr<Image> image, std::string path) {
    std::shared_ptr<Image> target;
    if (string_ends_with(path, ".png")) {
        if (typeid(image) == typeid(PNGImage)) {
            target = image;
        } else {
            target = std::make_shared<PNGImage>(*image);
        }
    } else if (string_ends_with(path, ".ppm")) {
        if (typeid(image) == typeid(PPMImage)) {
            target = image;
        } else {
            target = std::make_shared<PPMImage>(*image);
        }
    } else {
        throw std::runtime_error("unsupported image format: " + path);
    }
    target->save(path);
}