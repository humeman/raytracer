#include <math/pdf.hpp>
#include <math/vec3.hpp>
#include <macros.hpp>

#include <numbers>

Vec3 random_cos() {
    double r1 = RAND_DOUBLE();
    double r2 = RAND_DOUBLE();
    double phi = 2 * std::numbers::pi * r1;
    return Vec3(
        std::cos(phi) * std::sqrt(r2),
        std::sin(phi) * std::sqrt(r2),
        std::sqrt(1 - r2)
    );
}

COSPDF::COSPDF(const Vec3 &n) {
    c = n.to_unit();
    Vec3 temp = (std::fabs(c.a) > 0.9) ? Vec3(0, 1, 0) : Vec3(1, 0, 0);
    b = (c ^ temp).to_unit();
    a = c ^ b;
}

double COSPDF::value(const Vec3 &dir) const {
    return std::fmax(0, (dir.to_unit() * c) / std::numbers::pi);
}

Vec3 COSPDF::generate() const {
    Vec3 r = random_cos();
    return r.a * a + r.b * b + r.c * c;
}

double ObjectListPDF::value(const Vec3 &dir) const {
    double sum;
    for (auto &obj : objects) {
        sum += obj->pdf(origin, dir);
    }
    return sum / objects.size();
}

Vec3 ObjectListPDF::generate() const {
    int i = RAND_INT_RANGE(0, objects.size() - 1);
    return objects[i]->random(origin);
}

double MixPDF::value(const Vec3 &dir) const {
    double sum;
    for (auto &pdf : pdfs) {
        sum += pdf->value(dir);
    }
    return sum / pdfs.size();
}

Vec3 MixPDF::generate() const {
    int i = RAND_INT_RANGE(0, pdfs.size() - 1);
    return pdfs[i]->generate();
}