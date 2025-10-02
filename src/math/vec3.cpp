#include <math/vec3.hpp>
#include <cmath>

Vec3 Vec3::to_unit() const {
    float magnitude = this->magnitude();
    return Vec3(a / magnitude, b / magnitude, c / magnitude);
}

float Vec3::magnitude() const {
    return std::sqrt(a * a + b * b + c * c);
}

float Vec3::magnitude_squared() const {
    return a * a + b * b + c * c;
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

float operator*(const Vec3 &a, const Vec3 &b) {
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

Vec3 operator*(const Vec3 &a, const float &s) {
    return Vec3(a.a * s, a.b * s, a.c * s);
}

Vec3 operator*(const float &s, const Vec3 &a) {
    return Vec3(a.a * s, a.b * s, a.c * s);
}

void operator*=(Vec3 &a, const float &s) {
    a.a *= s;
    a.b *= s;
    a.c *= s;
}

Vec3 operator/(const Vec3 &a, const float &v) {
    return Vec3(a.a / v, a.b / v, a.c / v);
}

void operator/=(Vec3 &a, const float &v) {
    a.a /= v;
    a.b /= v;
    a.c /= v;
}