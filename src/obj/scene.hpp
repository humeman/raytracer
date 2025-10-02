#ifndef SCENE_HPP
#define SCENE_HPP

#include <obj/object.hpp>
#include <math/interval.hpp>

#include <vector>
#include <memory>

class Scene {
    private:
        std::vector<std::shared_ptr<Object>> objects;

    public:
        void add(std::shared_ptr<Object> obj);
        void remove(std::shared_ptr<Object> obj);

        bool hit(const Ray &r, const Interval &ray_t, HitResult &result) const;
};

#endif