#ifndef MACROS_HPP
#define MACROS_HPP

#define CLAMP(x, low, high) ((x < low) ? low : (x > high) ? high : x)
#define FLOAT_EQ(a, b) (std::fabs((a) - (b)) < 0.000005f)

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

#endif