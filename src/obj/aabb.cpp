#include <obj/aabb.hpp>

AABB::AABB(const AABB &a, const AABB &b) {
    x = Interval(a.x, b.x);
    y = Interval(a.y, b.y);
    z = Interval(a.z, b.z);
    pad();
}

const Interval &AABB::axis_interval(Direction &dir) const {
    if (dir == Direction::Y) return y;
    if (dir == Direction::Z) return z;
    return x;
}

bool AABB::hit(const Ray &r, Interval ray_t) const {
    const Vec3 &origin = r.get_origin();
    const Vec3 &direction = r.get_direction();

    for (int dir_i = Direction::X; dir_i <= Direction::Z; dir_i++) {
        Direction dir = static_cast<Direction>(dir_i);
        const Interval &ax = axis_interval(dir);
        const double inv_ad = 1.0 / direction.in_direction(dir);
        double t0 = (ax.min - origin.in_direction(dir)) * inv_ad;
        double t1 = (ax.max - origin.in_direction(dir)) * inv_ad;

        if (t0 < t1) {
            if (t0 > ray_t.min) ray_t.min = t0;
            if (t1 < ray_t.max) ray_t.max = t1;
        } else {
            if (t1 > ray_t.min) ray_t.min = t1;
            if (t0 < ray_t.max) ray_t.max = t0;
        }

        if (ray_t.max <= ray_t.min) return false;
    }
    return true;
}

Direction AABB::longest_axis() const {
    if (x.size() > y.size()) {
        return x.size() > z.size() ? Direction::X : Direction::Z;
    } else {
        return y.size() > z.size() ? Direction::Y : Direction::Z;
    }
}

const AABB AABB::empty = AABB(Interval::empty, Interval::empty, Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);

void AABB::pad() {
    if (x.size() < PAD_DELTA) x = x.grow(PAD_DELTA);
    if (y.size() < PAD_DELTA) y = y.grow(PAD_DELTA);
    if (z.size() < PAD_DELTA) z = z.grow(PAD_DELTA);
}