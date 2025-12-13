#pragma once

#include <math/ray.hpp>
#include <math/vec3.hpp>
#include <math/interval.hpp>
#include <obj/object.hpp>
#include <obj/aabb.hpp>
#include <memory>

class Triangle : public Object {
    private:
        Vec3 a, b, c;
        Vec3 uv_a, uv_b, uv_c;
        Vec3 normal;
        AABB box;

    public:
        Triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c, std::shared_ptr<Material> material) 
            : Triangle(a, b, c, Vec3(0, 0, 0), Vec3(1, 0, 0), Vec3(0, 1, 0), material) {}

        Triangle(const Vec3 &a, const Vec3 &b, const Vec3 &c, const Vec3 &uv_a, const Vec3 &uv_b, const Vec3 &uv_c, std::shared_ptr<Material> material)
            : Object(material), a(a), b(b), c(c), uv_a(uv_a), uv_b(uv_b), uv_c(uv_c) {
            normal = ((b - a) ^ (c - a)).to_unit();
            Interval x(a.a, a.a), y(a.b, a.b), z(a.c, a.c);
            if (b.a < x.min) x.min = b.a;
            if (c.a < x.min) x.min = c.a;
            if (b.b < y.min) y.min = b.b;
            if (c.b < y.min) y.min = c.b;
            if (b.c < z.min) z.min = b.c;
            if (c.c < z.min) z.min = c.c;
            if (b.a > x.max) x.max = b.a;
            if (c.a > x.max) x.max = c.a;
            if (b.b > y.max) y.max = b.b;
            if (c.b > y.max) y.max = c.b;
            if (b.c > z.max) z.max = b.c;
            if (c.c > z.max) z.max = c.c;
            box = AABB(x, y, z);
        }

        bool hit(const Ray &r, const Interval &ray_t, HitResult &res) const override;
        AABB bounding_box() const override;
};