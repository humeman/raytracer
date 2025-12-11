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
        int size();

        bool hit(const Ray &r, const Interval &ray_t, std::shared_ptr<Object> &res_obj, double &res_t, Vec3 &res_point, Vec3 &res_normal) const;
};

#endif