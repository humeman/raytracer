#include <math/interval.hpp>


float Interval::size() const {
    return max - min;
}
bool Interval::contains(float x) const {
    return x >= min && x <= max;
}
bool Interval::surrounds(float x) const {
    return x > min && x < max;
}

const Interval Interval::empty = Interval();
const Interval Interval::universe = Interval(-FLOAT_INFINITY, FLOAT_INFINITY);