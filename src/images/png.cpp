#include <images/png.hpp>
#include <macros.hpp>

#include <fstream>

#define MISSING_PNGPP_ERR std::runtime_error("install PNG++ before building for PNG support");

PNGImage::PNGImage(std::string path) {
    #ifdef PNGPP
    data = new png::image<png::rgba_pixel>(path);
    w = data->get_width();
    h = data->get_height();
    #else
    (void) path;
    throw MISSING_PNGPP_ERR;
    #endif
}

PNGImage::PNGImage(int width, int height) {
    #ifdef PNGPP
    w = width;
    h = height;
    data = new png::image<png::rgba_pixel>(w, h);
    #else
    (void) width; (void) height;
    throw MISSING_PNGPP_ERR;
    #endif
}

PNGImage::PNGImage(Image &image) {
    #ifdef PNGPP
    w = image.width();
    h = image.height();
    data = new png::image<png::rgba_pixel>(w, h);
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            data->set_pixel(x, y, png::rgba_pixel(
                (uint8_t) (image.at(x, y, Channel::R) * 255.999f),
                (uint8_t) (image.at(x, y, Channel::G) * 255.999f),
                (uint8_t) (image.at(x, y, Channel::B) * 255.999f),
                255));
        }
    }
    #else
    (void) image;
    throw MISSING_PNGPP_ERR;
    #endif
}

PNGImage::~PNGImage() {
    #ifdef PNGPP
    delete data;
    #endif
}

void PNGImage::save(std::string path) {
    #ifdef PNGPP
    data->write(path);
    #else
    (void) path;
    throw MISSING_PNGPP_ERR;
    #endif
}

int PNGImage::width() {
    #ifdef PNGPP
    return w;
    #else
    throw MISSING_PNGPP_ERR;
    #endif
}

int PNGImage::height() {
    #ifdef PNGPP
    return h;
    #else
    throw MISSING_PNGPP_ERR;
    #endif
}

float PNGImage::at(int x, int y, Channel channel) {
    #ifdef PNGPP
    if (x >= w || x < 0 || y >= h || y < 0) {
        throw std::runtime_error("coordinate " + std::to_string(x) + "x" + std::to_string(y) + " out of bounds (img " + std::to_string(w) + "x" + std::to_string(h) + ")");
    }
    switch (channel) {
        case R:
            return data->get_pixel(x, y).red / 255.999f;
        case G:
            return data->get_pixel(x, y).green / 255.999f;
        case B:
            return data->get_pixel(x, y).blue / 255.999f;
        default:
            return 0.0f; // impossible
    }
    #else
    (void) x; (void) y; (void) channel;
    throw MISSING_PNGPP_ERR;
    #endif
}

void PNGImage::set(int x, int y, Channel channel, float value) {
    #ifdef PNGPP
    if (x >= w || x < 0 || y >= h || y < 0) {
        throw std::runtime_error("coordinate " + std::to_string(x) + "x" + std::to_string(y) + " out of bounds (img " + std::to_string(w) + "x" + std::to_string(h) + ")");
    }
    if (value < 0.0f || value > 1.0f) {
        throw std::runtime_error("value " + std::to_string(value) + " out of range of [0.0, 1.0)");
    }
    png::rgba_pixel pixel = data->get_pixel(x, y);
    switch (channel) {
        case R:
            pixel.red = (uint8_t) (value * 255.999f);
            break;
        case G:
            pixel.green = (uint8_t) (value * 255.999f);
            break;
        case B:
            pixel.blue = (uint8_t) (value * 255.999f);
            break;
    }
    data->set_pixel(x, y, pixel);
    #else
    (void) x; (void) y; (void) channel; (void) value;
    throw MISSING_PNGPP_ERR;
    #endif
}