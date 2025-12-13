#pragma once

#include <math/interval.hpp>
#include <math/vec3.hpp>
#include <math/ray.hpp>
#include <obj/object.hpp>
#include <obj/scene.hpp>
#include <obj/aabb.hpp>

class BVHNode : public Object {
    private:
        std::shared_ptr<Object> left;
        std::shared_ptr<Object> right;
        AABB bbox;
    public:
        BVHNode(Scene &scene) : BVHNode(scene, 0, scene.size()) {}
        BVHNode(Scene &scene, size_t start, size_t end);

        bool hit(const Ray &r, const Interval &ray_t, HitResult &res) const override;
        AABB bounding_box() const override;

        static bool box_compare(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b, Direction axis);
        static bool box_compare_x(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b);
        static bool box_compare_y(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b);
        static bool box_compare_z(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b);
};