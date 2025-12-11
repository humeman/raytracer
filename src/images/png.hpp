#ifndef PNG_HPP
#define PNG_HPP

#include <images/image.hpp>
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
        double at(int x, int y, Channel channel) override;
        Color at(int x, int y) override;
        void set(int x, int y, Channel channel, double value) override;
        void set(int x, int y, Color color) override;
};

#endif