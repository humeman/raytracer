#ifndef INTERVAL_H
#define INTERVAL_H

#include <macros.hpp>

class Interval {
    public:
        float min;
        float max;

        Interval() : min(INFINITY), max(-INFINITY) {}
        Interval(float min, float max) : min(min), max(max) {}

        float size() const;
        bool contains(float x) const;
        bool surrounds(float x) const;

        static const Interval empty, universe;
};

#endif