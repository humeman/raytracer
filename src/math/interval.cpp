#include <math/interval.hpp>


Interval::Interval() : min(DOUBLE_INFINITY), max(-DOUBLE_INFINITY) {
    distribution = nullptr;
}
Interval::Interval(double min, double max) : min(min), max(max) {
    distribution = nullptr;
}
Interval::Interval(const Interval &a, const Interval &b) {
    min = a.min <= b.min ? a.min : b.min;
    max = a.max >= b.max ? a.max : b.max;
    distribution = nullptr;
}
Interval::~Interval() {
    if (distribution != nullptr) {
        delete distribution;
        distribution = nullptr;
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
        distribution = new std::uniform_real_distribution<double>(min, max);
    }
    return (*distribution)(generator);
}
Interval Interval::grow(double amount) const {
    double per = amount / 2;
    return Interval(
        min - per,
        max + per
    );
}

Interval Interval::empty = Interval();
Interval Interval::universe = Interval(-DOUBLE_INFINITY, DOUBLE_INFINITY);
Interval Interval::epsilon = Interval(-EPSILON, EPSILON);
Interval Interval::one = Interval(0, 1);
