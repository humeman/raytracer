#pragma once

#include <math/ray.hpp>
#include <math/vec3.hpp>
#include <math/interval.hpp>
#include <obj/object.hpp>
#include <obj/aabb.hpp>
#include <memory>

class Quad : public Object {
    private:
        Vec3 origin, u, v, w, normal;
        double d, area;
        AABB box;

    public:
        Quad(const Vec3 &origin, const Vec3 &u, const Vec3 &v, std::shared_ptr<Material> material) 
            : Object(material), origin(origin), u(u), v(v) {
            Vec3 n = u ^ v;
            normal = n.to_unit();
            d = normal * origin;
            w = n / (n * n);
            area = n.magnitude();

            AABB diag1(origin, origin + u + v);
            AABB diag2(origin + u, origin + v);
            box = AABB(diag1, diag2);
        }

        bool hit(const Ray &r, const Interval &ray_t, HitResult &res) const override;
        AABB bounding_box() const override;
        double pdf(const Vec3 &origin, const Vec3 &direction) const override;
        Vec3 random(const Vec3 &origin) const override;
};