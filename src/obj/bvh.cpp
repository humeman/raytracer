#include <obj/bvh.hpp>

#include <algorithm>


BVHNode::BVHNode(Scene &scene, size_t start, size_t end) : Object(nullptr) {
    bbox = AABB::empty;
    for (size_t i = start; i < end; i++) {
        bbox = AABB(bbox, scene.objects[i]->bounding_box());
    }
    Direction dir = bbox.longest_axis();
    auto comp = (dir == Direction::X) ? box_compare_x :
                (dir == Direction::Y) ? box_compare_y :
                                        box_compare_z;

    size_t span = end - start;
    if (span == 1) {
        left = right = scene.objects[start];
    } else if (span == 2) {
        left = scene.objects[start];
        right = scene.objects[start + 1];
    } else {
        std::sort(std::begin(scene.objects) + start, std::begin(scene.objects) + end, comp);
        auto mid = start + span / 2;
        left = std::make_shared<BVHNode>(scene, start, mid);
        right = std::make_shared<BVHNode>(scene, mid, end);
    }
    bbox = AABB(left->bounding_box(), right->bounding_box());
}

bool BVHNode::hit(const Ray &r, const Interval &ray_t, HitResult &res) const {
    if (!bbox.hit(r, ray_t)) return false;
    bool hit_l = left->hit(r, ray_t, res);
    bool hit_r = right->hit(r, Interval(ray_t.min, hit_l ? res.t : ray_t.max), res);
    return hit_l || hit_r;
}

AABB BVHNode::bounding_box() const {
    return bbox;
}

bool BVHNode::box_compare(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b, Direction axis) {
    auto a_interval = a->bounding_box().axis_interval(axis);
    auto b_interval = b->bounding_box().axis_interval(axis);
    return a_interval.min < b_interval.min;
}
bool BVHNode::box_compare_x(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b) {
    return box_compare(a, b, Direction::X);
}
bool BVHNode::box_compare_y(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b) {
    return box_compare(a, b, Direction::Y);
}
bool BVHNode::box_compare_z(const std::shared_ptr<Object> a, const std::shared_ptr<Object> b) {
    return box_compare(a, b, Direction::Z);
}