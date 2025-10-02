#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <math/ray.hpp>
#include <math/interval.hpp>

class HitResult {
    public:
        Vec3 point;
        Vec3 normal;
        float t;
        bool front_face;

        void set_face_normal(const Ray &r, const Vec3 &outward_normal);
};

class Object {
    public:
        virtual ~Object() = default;
        virtual bool hit(const Ray &r, const Interval &ray_t, HitResult &result) const = 0;
};

#endif