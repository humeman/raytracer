#pragma once

#include <math/ray.hpp>
#include <math/vec3.hpp>
#include <math/interval.hpp>
#include <obj/object.hpp>
#include <obj/aabb.hpp>
#include <memory>

class CDM : public Object {
    private:
        std::shared_ptr<Object> boundary;
        double density;

    public:
        CDM(std::shared_ptr<Object> boundary, double density)
            : Object(boundary->material), boundary(boundary), density(density) {}

        bool hit(const Ray &r, const Interval &ray_t, HitResult &res) const override;
        AABB bounding_box() const override;
};