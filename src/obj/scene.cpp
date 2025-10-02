#include <obj/scene.hpp>
#include <macros.hpp>

#include <algorithm>

void Scene::add(std::shared_ptr<Object> obj) {
    objects.push_back(obj);
}
void Scene::remove(std::shared_ptr<Object> obj) {
    // https://stackoverflow.com/a/3385251
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

bool Scene::hit(const Ray &r, const Interval &ray_t, HitResult &result) const {
    bool found = false;
    float closest = ray_t.max;
    HitResult temp;

    for (const std::shared_ptr<Object> &obj : objects) {
        if (obj->hit(r, Interval(ray_t.min, closest), temp)) {
            result = temp;
            found = true;
            closest = result.t;
        }
    }
    return found;
}