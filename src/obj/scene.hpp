#ifndef SCENE_HPP
#define SCENE_HPP

#include <vector>
#include <obj/object.hpp>

class Scene {
    private:
        std::vector<Object *> objects;

    public:
        Scene() = default;
        ~Scene();

        void add(Object *obj);
        void remove(Object *obj);

        Color color(const Ray &r) const;
};

#endif