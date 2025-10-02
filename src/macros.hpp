#ifndef MACROS_HPP
#define MACROS_HPP

#define CLAMP(x, low, high) ((x < low) ? low : (x > high) ? high : x)
#define FLOAT_EQ(a, b) (std::fabs((a) - (b)) < 0.000005f)

// https://www.youtube.com/watch?v=dXqtrHJAqVM
#define CLOG(x) std::clog << "\33[2K\r" << x << " " << std::flush;

#endif