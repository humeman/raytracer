#pragma once

#include <math/vec3.hpp>
#include <obj/object.hpp>

#include <memory>

class PDF {
    public:
        virtual ~PDF() {}

        virtual double value(const Vec3 &dir) const = 0;
        virtual Vec3 generate() const = 0;
};

class COSPDF : public PDF {
    private:
        Vec3 a, b, c;
    public:
        COSPDF(const Vec3 &w);

        double value(const Vec3 &dir) const override;
        Vec3 generate() const override;
};

class Object;

class ObjectListPDF : public PDF {
    private:
        const std::vector<std::shared_ptr<Object>> &objects;
        const Vec3 origin;

    public:
        ObjectListPDF(const std::vector<std::shared_ptr<Object>> &objects, const Vec3 origin) : objects(objects), origin(origin) {}

        double value(const Vec3 &dir) const override;
        Vec3 generate() const override;
};

class MixPDF : public PDF {
    private:
        std::vector<std::shared_ptr<PDF>>  pdfs;

    public:
        MixPDF(std::shared_ptr<PDF> a, std::shared_ptr<PDF> b) 
            : pdfs({a, b}) {}
        MixPDF(std::vector<std::shared_ptr<PDF>> pdfs) : pdfs(pdfs) {}

        double value(const Vec3 &dir) const override;
        Vec3 generate() const override;
};