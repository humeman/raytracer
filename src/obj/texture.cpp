#include <obj/texture.hpp>

Color ColorTexture::value(double u, double v, const Vec3 &point) const {
    (void) u;
    (void) v;
    (void) point;
    return albedo;
}

Color ImageTexture::value(double u, double v, const Vec3 &point) const {
    (void) point;
    u = Interval::one.clamp(u);
    v = 1.0 - Interval::one.clamp(v);

    int x = (int) (u * image->width());
    int y = (int) (v * image->height());
    return image->at(x, y);
}