#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <math/ray.hpp>
#include <math/vec3.hpp>
#include <math/interval.hpp>
#include <obj/object.hpp>
#include <memory>

class Sphere : public Object {
    private:
        Vec3 center;
        double radius;

    public:
        Sphere(const Vec3 &center, const double &radius, std::shared_ptr<Material> material) : Object(material), center(center), radius(radius) {}

        bool hit(const Ray &r, const Interval &ray_t, double &res_t, Vec3 &res_point, Vec3 &res_normal) const override;
};

#endif