#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <string>
#include <cstdint>

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
         * Saves the image to a file.
         * @param path The path to save to.
         */
        virtual void save(std::string path) = 0;

        /**
         * Gets the width of the image.
         * @return The width of the image.
         */
        virtual int width() = 0;

        /**
         * Gets the height of the image.
         * @return The height of the image.
         */
        virtual int height() = 0;

        /**
         * Gets the value of a pixel at a given coordinate.
         * @param x The x coordinate of the pixel.
         * @param y The y coordinate of the pixel.
         * @param channel The channel to get.
         * @return The value of the channel at the given coordinate, in the range [0.0, 1.0).
         */
        virtual float at(int x, int y, Channel channel) = 0;

        /**
         * Sets the value of a pixel at a given coordinate.
         * @param x The x coordinate of the pixel.
         * @param y The y coordinate of the pixel.
         * @param channel The channel to set.
         * @param value The value to set the channel to, in the range [0.0, 1.0).
         */
        virtual void set(int x, int y, Channel channel, float value) = 0;

        static void write_any(Image &image, std::string path);
        static Image *read_any(std::string path);
};

#endif