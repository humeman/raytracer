#pragma once

#include <math/vec3.hpp>
#include <memory>

class Material;

// needs to be separate for a circular dependency
class HitResult {
    public:
        Vec3 point;
        Vec3 normal;
        std::shared_ptr<Material> material;
        double t;
        double u, v;
        bool is_front_face;
};