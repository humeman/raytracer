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

bool Scene::hit(const Ray &r, const Interval &ray_t, std::shared_ptr<Object> &res_obj, double &res_t, Vec3 &res_point, Vec3 &res_normal) const {
    bool found = false;
    double closest = ray_t.max;
    double temp_t;
    Vec3 temp_point, temp_normal;

    for (const std::shared_ptr<Object> &obj : objects) {
        if (obj->hit(r, Interval(ray_t.min, closest), temp_t, temp_point, temp_normal)) {
            res_t = temp_t;
            res_point = temp_point;
            res_normal = temp_normal;
            res_obj = obj;
            found = true;
            closest = temp_t;
        }
    }
    return found;
}

int Scene::size() {
    return objects.size();
}