#ifndef SCENE_HPP
#define SCENE_HPP

#include <obj/object.hpp>
#include <obj/aabb.hpp>
#include <math/interval.hpp>

#include <vector>
#include <memory>

class Scene {
    private:
        std::vector<std::shared_ptr<Object>> objects;
        AABB bbox;

        friend class BVHNode;

    public:
        Scene() {}
        Scene(std::shared_ptr<Object> obj) { 
            add(obj); 
        }
        void add(std::shared_ptr<Object> obj);
        void remove(std::shared_ptr<Object> obj);
        int size() const;
        AABB bounding_box() const;

        bool hit(const Ray &r, const Interval &ray_t, HitResult &res) const;
};

#endif