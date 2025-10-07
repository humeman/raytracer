#include <math/interval.hpp>


Interval::Interval() : min(DOUBLE_INFINITY), max(-DOUBLE_INFINITY) {
    distribution = nullptr;
}
Interval::Interval(double min, double max) : min(min), max(max) {
    distribution = new std::uniform_real_distribution<double>(min, max);
}
Interval::~Interval() {
    if (distribution != nullptr) {
        delete distribution;
    }
}

double Interval::size() const {
    return max - min;
}
bool Interval::contains(double x) const {
    return x >= min && x <= max;
}
bool Interval::surrounds(double x) const {
    return x > min && x < max;
}
double Interval::clamp(double in) const {
    return CLAMP(in, min, max);
}
double Interval::random() {
    if (distribution == nullptr) {
        throw EXC("can't use random() with an empty interval");
    }
    double rand = (*distribution)(generator);
    return min + (max - min) * rand;
}

Interval Interval::empty = Interval();
Interval Interval::universe = Interval(-DOUBLE_INFINITY, DOUBLE_INFINITY);
