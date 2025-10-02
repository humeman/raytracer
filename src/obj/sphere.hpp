#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <math/ray.hpp>
#include <math/vec3.hpp>
#include <obj/object.hpp>

class Sphere : public Object {
    public:
        Vec3 center;
        float radius;

        Sphere(const Vec3 &center, const float &radius) : center(center), radius(radius) {}

        bool hit(const Ray &r, Color &res) const override;
};

#endif