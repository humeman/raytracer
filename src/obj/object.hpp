#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <math/ray.hpp>

class Object {
    public:
        virtual ~Object() = default;
        virtual bool hit(const Ray &r, Color &res) const = 0;
};

#endif