#ifndef PNG_HPP
#define PNG_HPP

#include "image.hpp"
#include <cstdint>
#include <png++/png.hpp>

/**
 * PNG image format.
 */
class PNGImage : public Image {
    private:
        int w;
        int h;

        png::image<png::rgba_pixel> *data;

    public:
        PNGImage(std::string path);
        PNGImage(int width, int height);
        PNGImage(Image &image);
        ~PNGImage();
        void save(std::string path) override;
        int width() override;
        int height() override;
        float at(int x, int y, Channel channel) override;
        void set(int x, int y, Channel channel, float value) override;
};

#endif