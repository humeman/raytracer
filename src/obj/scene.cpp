#include <obj/scene.hpp>
#include <macros.hpp>

#include <algorithm>

void Scene::add(std::shared_ptr<Object> obj) {
    objects.push_back(obj);
    bbox = AABB(bbox, obj->bounding_box());
}
void Scene::remove(std::shared_ptr<Object> obj) {
    // https://stackoverflow.com/a/3385251
    objects.erase(std::remove(objects.begin(), objects.end(), obj), objects.end());
}

bool Scene::hit(const Ray &r, const Interval &ray_t, HitResult &res) const {
    bool found = false;
    double closest = ray_t.max;
    HitResult temp_res;

    for (const std::shared_ptr<Object> &obj : objects) {
        if (obj->hit(r, Interval(ray_t.min, closest), temp_res)) {
            res = temp_res;
            found = true;
            closest = temp_res.t;
        }
    }
    return found;
}

int Scene::size() const {
    return objects.size();
}

AABB Scene::bounding_box() const {
    return bbox;
}