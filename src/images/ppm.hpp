#ifndef PPM_HPP
#define PPM_HPP

#include <images/image.hpp>
#include <cstdint>

/**
 * PPM image format.
 */
class PPMImage : public Image {
    private:
        int w;
        int h;
        bool binary;

        float *data;

    public:
        PPMImage(std::string path);
        PPMImage(int width, int height);
        PPMImage(Image &image);
        ~PPMImage();
        void save(std::string path) override;
        int width() override;
        int height() override;
        float at(int x, int y, Channel channel) override;
        void set(int x, int y, Channel channel, float value) override;

        void set_binary(bool binary);
};

#endif