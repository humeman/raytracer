#ifndef SPHERE_HPP
#define SPHERE_HPP

#include <math/ray.hpp>
#include <math/vec3.hpp>
#include <math/interval.hpp>
#include <obj/object.hpp>
#include <obj/aabb.hpp>
#include <memory>

class Sphere : public Object {
    private:
        Ray center;
        double radius;
        AABB box;

    public:
        Sphere(const Vec3 &center, const double &radius, std::shared_ptr<Material> material) : Object(material), center(center, Vec3(0, 0, 0)), radius(radius) {
            Vec3 r = Vec3(radius, radius, radius);
            box = AABB(center - r, center + r);
        }
        Sphere(const Vec3 &center_start, const Vec3 &center_end, const double &radius, std::shared_ptr<Material> material) : Object(material), center(center_start, center_end - center_start), radius(radius) {
            auto r = Vec3(radius, radius, radius);
            box = AABB(
                AABB(center.at(0) - r, center.at(0) + r),
                AABB(center.at(1) - r, center.at(1) + r)
            );
        }


        bool hit(const Ray &r, const Interval &ray_t, HitResult &res) const override;
        AABB bounding_box() const override;

    private:
        static void uv(const Vec3 &p, double &u, double &v);
};

#endif