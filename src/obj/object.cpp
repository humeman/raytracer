#include <obj/object.hpp>

void HitResult::set_face_normal(const Ray &r, const Vec3 &outward_normal) {
    front_face = (r.get_direction() * outward_normal) < 0;
    normal = front_face ? outward_normal : -1 * outward_normal;
}