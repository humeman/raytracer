#ifndef INTERVAL_H
#define INTERVAL_H

#include <macros.hpp>

class Interval {
    private:
        std::uniform_real_distribution<double> *distribution;
        std::mt19937 generator;

    public:
        double min;
        double max;

        Interval();
        Interval(double min, double max);
        Interval(const Interval &a, const Interval &b);
        ~Interval();

        double size() const;
        bool contains(double x) const;
        bool surrounds(double x) const;
        double clamp(double in) const;
        double random();
        Interval grow(double amount) const;

        static Interval empty, universe, epsilon, one;
};

#endif