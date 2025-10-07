#include <math/vec3.hpp>
#include <cmath>

Vec3 Vec3::to_unit() const {
    double magnitude = this->magnitude();
    return Vec3(a / magnitude, b / magnitude, c / magnitude);
}

double Vec3::magnitude() const {
    return std::sqrt(a * a + b * b + c * c);
}

double Vec3::magnitude_squared() const {
    return a * a + b * b + c * c;
}

bool Vec3::zero() const {
    return DOUBLE_EQ(a, 0.0) && DOUBLE_EQ(b, 0.0) && DOUBLE_EQ(c, 0.0);
}

Vec3 operator+(const Vec3 &a, const Vec3 &b) {
    return Vec3(a.a + b.a, a.b + b.b, a.c + b.c);
}

void operator+=(Vec3 &a, const Vec3 &b) {
    a.a += b.a;
    a.b += b.b;
    a.c += b.c;
}

Vec3 operator-(const Vec3 &a, const Vec3 &b) {
    return Vec3(a.a - b.a, a.b - b.b, a.c - b.c);
}

void operator-=(Vec3 &a, const Vec3 &b) {
    a.a -= b.a;
    a.b -= b.b;
    a.c -= b.c;
}

double operator*(const Vec3 &a, const Vec3 &b) {
    return a.a * b.a + a.b * b.b + a.c * b.c;
}

void operator*=(Vec3 &a, const Vec3 &b) {
    a.a *= b.a;
    a.b *= b.b;
    a.c *= b.c;
}

Vec3 operator^(const Vec3 &a, const Vec3 &b) {
    return Vec3(
        a.b * b.c - a.c * b.b,
        a.c * b.a - a.a * b.c,
        a.a * b.b - a.b * b.a
    );
}

void operator^=(Vec3 &a, const Vec3 &b) {
    a = a ^ b;
}

Vec3 operator*(const Vec3 &a, const double &s) {
    return Vec3(a.a * s, a.b * s, a.c * s);
}

Vec3 operator*(const double &s, const Vec3 &a) {
    return Vec3(a.a * s, a.b * s, a.c * s);
}

void operator*=(Vec3 &a, const double &s) {
    a.a *= s;
    a.b *= s;
    a.c *= s;
}

Vec3 operator/(const Vec3 &a, const double &v) {
    return Vec3(a.a / v, a.b / v, a.c / v);
}

void operator/=(Vec3 &a, const double &v) {
    a.a /= v;
    a.b /= v;
    a.c /= v;
}

Vec3 Vec3::random() {
    return Vec3(Interval::universe.random(), Interval::universe.random(), Interval::universe.random());
}

Vec3 Vec3::random(Interval &range) {
    return Vec3(range.random(), range.random(), range.random());
}

Interval RANDOM_UNIT_INTERVAL(-1.0, 1.0);

Vec3 Vec3::random_unit() {
    while (true) {
        Vec3 p = random(RANDOM_UNIT_INTERVAL);
        double len = p.magnitude_squared();
        if (len <= 1.0 && len >= 1e-160) return p.to_unit();
    }
}

const Vec3 Vec3::white = Vec3(1.0, 1.0, 1.0);
const Vec3 Vec3::black = Vec3(0.0, 0.0, 0.0);