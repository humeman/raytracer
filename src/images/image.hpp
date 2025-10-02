#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <cstdint>
#include <math/vec3.hpp>

enum Channel {
    R = 0,
    G = 1,
    B = 2
};

#define IDX(w, x, y, c) ((y) * (w) * 3 + (x) * 3 + (c))

/**
 * Base class for an image, which will allow us to load/save different formats down the road.
 */
class Image {
    public:
        virtual ~Image() = default;

        /**
         * Saves the image to a file
         * @param path path to save to
         */
        virtual void save(std::string path) = 0;

        /**
         * @return width of the image
         */
        virtual int width() = 0;

        /**
         * @return height of the image
         */
        virtual int height() = 0;

        /**
         * Gets the value of a pixel at a given coordinate
         * @param x
         * @param y
         * @param channel channel to get
         * @return channel value, [0.0, 1.0]
         */
        virtual float at(int x, int y, Channel channel) = 0;

        /**
         * Sets the value of a pixel
         * @param x
         * @param y
         * @param channel channel to set
         * @param value value to set the channel to, [0.0, 1.0]
         */
        virtual void set(int x, int y, Channel channel, float value) = 0;

        /**
         * Sets the color of a pixel
         * @param x
         * @param y
         * @param color color to set the pixel to, each [0.0, 1.0]
         */
        virtual void set(int x, int y, Color color) = 0;

        static void write_any(Image &image, std::string path);
        static Image *read_any(std::string path);
};

#endif