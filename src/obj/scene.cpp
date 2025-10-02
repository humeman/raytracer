#include "scene.hpp"

#include <algorithm>

Scene::~Scene() {
    for (Object *obj : objects) {
        delete obj;
    }
}

void Scene::add(Object *obj) {
    objects.push_back(obj);
}
void Scene::remove(Object *obj) {
    // https://stackoverflow.com/a/3385251
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

Color Scene::color(const Ray &r) const {
    Color res;
    for (const Object *obj : objects) {
        if (obj->hit(r, res)) {
            return res;
        }
    }

    Vec3 unit_direction = r.get_direction().to_unit();
    float a = 0.5f * (unit_direction.b + 1.0f);

    res = (1.0f - a) * Color(1.0f, 1.0f, 1.0f) + a * Color(0.5f, 0.7f, 1.0f);
    return res;
}