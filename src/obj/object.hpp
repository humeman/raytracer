#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <math/ray.hpp>
#include <math/interval.hpp>
#include <obj/material.hpp>

#include <memory>

class Object {
    protected:
        Object(std::shared_ptr<Material> material) : material(material) {}

    public:
        std::shared_ptr<Material> material;

        virtual ~Object() = default;
        virtual bool hit(const Ray &r, const Interval &ray_t, double &res_t, Vec3 &res_point, Vec3 &res_normal) const = 0;
};

#endif