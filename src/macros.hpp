#ifndef MACROS_HPP
#define MACROS_HPP

#include <limits>
#include <stdexcept>
#include <numbers>
#include <random>

#define CLAMP(x, low, high) ((x < low) ? low : ((x > high) ? high : x))
#define DOUBLE_EQ(a, b) (std::fabs((a) - (b)) < 0.000005)

// https://www.youtube.com/watch?v=dXqtrHJAqVM
#define CLOG(x) std::clog << "\33[2K\r" << x << " " << std::flush;

class TracerException : public std::exception {
    private:
        std::string message;
    public:
        TracerException(const char *function, std::string message) {
            this->message = "\n  - " + std::string(function) + ": " + message;
        }
        TracerException(const char *function, TracerException &e) {
            this->message = std::string(e.what()) + "\n  - " + std::string(function);
        }
        TracerException(const char *function, TracerException &e, std::string message) {
            this->message = std::string(e.what()) + "\n  - " + std::string(function) + ": " + message;
        }
        TracerException(const char *function, const char *message) {
            this->message = "\n  - " + std::string(function) + ": " + std::string(message);
        }
        TracerException(const char *function, TracerException &e, const char *message) {
            this->message = std::string(e.what()) + "\n  - " + function + ": " + std::string(message);
        }

        const char *what() const noexcept override {
            return message.c_str();
        }
};

// https://stackoverflow.com/a/5459929
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)
#define EXC(...) TracerException(__FILE__ ":" STR(__LINE__), ##__VA_ARGS__)

#define DOUBLE_INFINITY std::numeric_limits<double>::infinity()
#define DEG_TO_RAD(x) ((x) * std::numbers::pi / 180.0)
#define RAD_TO_DEG(x) ((x) * 180.0 / std::numbers::pi)
#define ON_HEMISPHERE(normal, vector) (((normal) * (vector)) > 0.0 ? (vector) : (vector * -1.0))
#define REFLECT(v, n) ((v) - 2 * ((v) * (n)) * (n))

#define LINEAR_TO_GAMMA(x) ((x) > 0 ? std::sqrt(x) : 0)

#endif