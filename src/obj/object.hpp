#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <math/ray.hpp>
#include <math/interval.hpp>
#include <obj/material.hpp>
#include <obj/aabb.hpp>
#include <obj/hit.hpp>

#include <memory>


class Object {
    protected:
        Object(std::shared_ptr<Material> material) : material(material) {}

    public:
        std::shared_ptr<Material> material;

        virtual ~Object() = default;
        virtual bool hit(const Ray &r, const Interval &ray_t, HitResult &res) const = 0;
        virtual AABB bounding_box() const = 0;

        virtual double pdf(const Vec3 &origin, const Vec3 &direction) const {
            (void) origin;
            (void) direction;
            return 0;
        }

        virtual Vec3 random(const Vec3 &origin) const {
            (void) origin;
            return Vec3(1, 0, 0);
        }
};

#endif