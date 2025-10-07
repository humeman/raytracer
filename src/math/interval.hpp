#ifndef INTERVAL_H
#define INTERVAL_H

#include <macros.hpp>

class Interval {
    private:
        std::uniform_real_distribution<double> *distribution;
        std::mt19937 generator;

    public:
        const double min;
        const double max;

        Interval();
        Interval(double min, double max);
        ~Interval();

        double size() const;
        bool contains(double x) const;
        bool surrounds(double x) const;
        double clamp(double in) const;
        double random();

        static Interval empty, universe;
};

#endif