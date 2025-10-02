#include "images/image.hpp"
#include "macros.hpp"

#include <stdexcept>
#include <iostream>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        throw std::runtime_error("no input file specified");
    }
    Image *img = Image::read_any(argv[1]);
    CLOG("Writing " << argv[2] << "...");
    Image::write_any(*img, argv[2]);
    CLOG("Done!");
    std::cout << std::endl;
    delete img;
    return 0;
}