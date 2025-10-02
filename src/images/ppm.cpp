#include <images/ppm.hpp>
#include <macros.hpp>

#include <fstream>

PPMImage::PPMImage(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("could not open " + path);
    }
    std::string format;
    file >> format;
    if (format == "P3") {
        binary = false;
    } else if (format == "P6") {
        binary = true;
    } else {
        throw std::runtime_error("not a supported PPM: " + format);
    }
    std::string word;
    file >> word;
    w = std::stoi(word);
    file >> word;
    h = std::stoi(word);
    file >> word;
    if (word != "255") {
        throw std::runtime_error("color depth isn't supported: " + word);
    }
    if (binary) file.get();
    data = new float[w * h * 3];
    uint8_t byte;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (Channel c : {R, G, B}) {
                if (binary) {
                    file.read((char *) &byte, 1);
                    data[IDX(w, x, y, c)] = byte / 256.0f;
                }
                else {
                    file >> word;
                    data[IDX(w, x, y, c)] = std::stoi(word) / 256.0f;
                }
            }
        }
    }
    file.close();
}

PPMImage::PPMImage(int width, int height) {
    w = width;
    h = height;
    binary = true;
    data = new float[w * h * 3]();
}

PPMImage::PPMImage(Image &image) {
    w = image.width();
    h = image.height();
    binary = true;
    data = new float[w * h * 3];
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (Channel c : {R, G, B}) {
                data[IDX(w, x, y, c)] = image.at(x, y, c);
            }
        }
    }
}

PPMImage::~PPMImage() {
    delete[] data;
}

void PPMImage::save(std::string path) {
    std::ofstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("could not open " + path);
    }

    file << (binary ? "P6" : "P3") << "\n";
    file << w << " " << h << "\n";
    file << "255\n";
    uint8_t value;
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            for (Channel c : {R, G, B}) {
                value = (uint8_t) (data[IDX(w, x, y, c)] * 255.999f);
                if (binary) {
                    file.write((char *) &value, 1);
                } else {
                    file << value << " ";
                }
            }
        }
    }
    file.close();
}

int PPMImage::width() {
    return w;
}

int PPMImage::height() {
    return h;
}

float PPMImage::at(int x, int y, Channel channel) {
    if (x >= w || x < 0 || y >= h || y < 0) {
        throw std::runtime_error("coordinate " + std::to_string(x) + "x" + std::to_string(y) + " out of bounds (img " + std::to_string(w) + "x" + std::to_string(h) + ")");
    }
    return data[IDX(w, x, y, channel)];
}

void PPMImage::set(int x, int y, Channel channel, float value) {
    if (x >= w || x < 0 || y >= h || y < 0) {
        throw std::runtime_error("coordinate " + std::to_string(x) + "x" + std::to_string(y) + " out of bounds (img " + std::to_string(w) + "x" + std::to_string(h) + ")");
    }
    if (value < 0.0f || value > 1.0f) {
        throw std::runtime_error("value " + std::to_string(value) + " out of range of [0.0, 1.0)");
    }
    data[IDX(w, x, y, channel)] = value;
}

void PPMImage::set(int x, int y, Color color) {
    if (x >= w || x < 0 || y >= h || y < 0) {
        throw std::runtime_error("coordinate " + std::to_string(x) + "x" + std::to_string(y) + " out of bounds (img " + std::to_string(w) + "x" + std::to_string(h) + ")");
    }
    if (color.a < 0.0f || color.a > 1.0f || color.b < 0.0f || color.b > 1.0f || color.c < 0.0f || color.c > 1.0f) {
        throw std::runtime_error("color out of range of [0.0, 1.0)");
    }
    data[IDX(w, x, y, R)] = color.a;
    data[IDX(w, x, y, G)] = color.b;
    data[IDX(w, x, y, B)] = color.c;
}

void PPMImage::set_binary(bool binary) {
    this->binary = binary;
}