#ifndef VEC3_HPP
#define VEC3_HPP

#include <math/interval.hpp>

class Vec3 {
    public:
        double a;
        double b;
        double c;

        Vec3() : a(0.0), b(0.0), c(0.0) {}
        Vec3(double a, double b, double c) : a(a), b(b), c(c) {}

        Vec3 to_unit() const;
        double magnitude() const;
        double magnitude_squared() const;
        bool zero() const;

        friend Vec3 operator+(const Vec3 &a, const Vec3 &b);
        friend void operator+=(Vec3 &a, const Vec3 &b);
        friend Vec3 operator-(const Vec3 &a, const Vec3 &b);
        friend void operator-=(Vec3 &a, const Vec3 &b);
        friend double operator*(const Vec3 &a, const Vec3 &b);
        friend void operator*=(Vec3 &a, const Vec3 &b);
        friend Vec3 operator%(const Vec3 &a, const Vec3 &b); // multiply by element
        friend void operator%=(Vec3 &a, const Vec3 &b);
        friend Vec3 operator^(const Vec3 &a, const Vec3 &b); // cross product
        friend void operator^=(Vec3 &a, const Vec3 &b);
        friend Vec3 operator*(const Vec3 &a, const double &s);
        friend Vec3 operator*(const double &s, const Vec3 &a);
        friend void operator*=(Vec3 &a, const double &s);
        friend Vec3 operator/(const Vec3 &a, const double &v);
        friend void operator/=(Vec3 &a, const double &v);

        static Vec3 random();
        static Vec3 random(Interval &range);
        static Vec3 random_unit();

        const static Vec3 white, black;
};

using Color = Vec3;
using Point = Vec3;

#endif