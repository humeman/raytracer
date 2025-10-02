#ifndef VEC3_HPP
#define VEC3_HPP

class Vec3 {
    public:
        float a;
        float b;
        float c;

        Vec3() : a(0.0f), b(0.0f), c(0.0f) {}
        Vec3(float a, float b, float c) : a(a), b(b), c(c) {}

        Vec3 to_unit();
        float magnitude();
        float magnitude_squared();

        friend Vec3 operator+(const Vec3 &a, const Vec3 &b);
        friend void operator+=(Vec3 &a, const Vec3 &b);
        friend Vec3 operator-(const Vec3 &a, const Vec3 &b);
        friend void operator-=(Vec3 &a, const Vec3 &b);
        friend Vec3 operator*(const Vec3 &a, const Vec3 &b);
        friend void operator*=(Vec3 &a, const Vec3 &b);
        friend Vec3 operator^(const Vec3 &a, const Vec3 &b); // cross product
        friend void operator^=(Vec3 &a, const Vec3 &b);
        friend Vec3 operator*(const Vec3 &a, const float &s);
        friend void operator*=(Vec3 &a, const float &s);
        friend Vec3 operator/(const Vec3 &a, const float &v);
        friend void operator/=(Vec3 &a, const float &v);
};

#endif