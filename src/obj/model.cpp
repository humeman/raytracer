// https://the-asset-importer-lib-documentation.readthedocs.io/en/latest/usage/use_the_lib.html

#include <obj/model.hpp>
#include <obj/triangle.hpp>
#include <macros.hpp>

#ifdef ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif

#include <iostream>


void ModelLoader::set_default_color(Color color) {
    default_color = color;
}

void rotate(Vec3 &point, Vec3 axis, double angle) {
    axis = axis.to_unit();
    // Didn't want to figure out quaternions. This is the matrix verison
    double rad = DEG_TO_RAD(angle);
    double cos = std::cos(rad);
    double sin = std::sin(rad);
    // I forgot what the symbol in the notes actually is, but it looks like
    // an n, so it's n now :)
    double n = (1 - cos);

    double x = (axis.a * axis.a * n + cos) * point.a
            + (axis.a * axis.b * n - axis.c * sin) * point.b
            + (axis.a * axis.c * n + axis.b * sin) * point.c;
    double y = (axis.a * axis.b * n + axis.c * sin) * point.a
            + (axis.b * axis.b * n + cos) * point.b
            + (axis.b * axis.c * n - axis.a * sin) * point.c;
    double z = (axis.a * axis.c * n - axis.b * sin) * point.a
            + (axis.b * axis.c * n + axis.a * sin) * point.b
            + (axis.c * axis.c * n + cos) * point.c;
    point.a = x;
    point.b = y;
    point.c = z;
}

void scale_and_translate(Vec3 &point, const Vec3 &translate, double scale) {
    point.a = (point.a * scale) + translate.a;
    point.b = (point.b * scale) + translate.b;
    point.c = (point.c * scale) + translate.c;
}

void update_minmax(Vec3 &min, Vec3 &max, Vec3 &check) {
    if (check.a < min.a) min.a = check.a;
    if (check.b < min.b) min.b = check.b;
    if (check.c < min.c) min.c = check.c;
    if (check.a > max.a) max.a = check.a;
    if (check.b > max.b) max.b = check.b;
    if (check.c > max.c) max.c = check.c;
}

void init_minmax(Vec3 &min, Vec3 &max, Vec3 &check) {
    min.a = check.a;
    min.b = check.b;
    min.c = check.c;
    max.a = check.a;
    max.b = check.b;
    max.c = check.c;
}

void ModelLoader::load(
        const std::string &path, 
        const Vec3 &origin, 
        double scale, 
        const Vec3 &rot_axis,
        double rot_angle) {
    #ifdef ASSIMP
    auto *ascene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_PreTransformVertices);

    if (!ascene) {
        throw EXC("assimp error for " + path + ": " + importer.GetErrorString());
    }

    bool set = false;
    Vec3 min;
    Vec3 max;

    for (uint i = 0; i < ascene->mNumMeshes; i++) {
        auto *mesh = ascene->mMeshes[i];

        for (uint j = 0; j < mesh->mNumFaces; j++) {
            auto face = mesh->mFaces[j];
            auto material = ascene->mMaterials[mesh->mMaterialIndex];

            Color color = default_color;
            if (material->GetTextureCount(aiTextureType_DIFFUSE) <= 0) {
                aiColor3D acolor(1, 1, 1);
                material->Get(AI_MATKEY_COLOR_DIFFUSE, acolor);
                color.a = acolor.r;
                color.b = acolor.g;
                color.c = acolor.b;
            }
            
            auto aa = mesh->mVertices[face.mIndices[0]];
            auto ab = mesh->mVertices[face.mIndices[1]];
            auto ac = mesh->mVertices[face.mIndices[2]];
            Vec3 a(aa[0], aa[1], aa[2]);
            Vec3 b(ab[0], ab[1], ab[2]);
            Vec3 c(ac[0], ac[1], ac[2]);

            if (rot_angle != 0 && rot_axis.magnitude_squared() != 0) {
                rotate(a, rot_axis, rot_angle);
                rotate(b, rot_axis, rot_angle);
                rotate(c, rot_axis, rot_angle);
            }

            scale_and_translate(a, origin, scale);
            scale_and_translate(b, origin, scale);
            scale_and_translate(c, origin, scale);

            if (!set) {
                init_minmax(min, max, a);
                update_minmax(min, max, b);
                update_minmax(min, max, c);
                set = true;
            } else {
                update_minmax(min, max, a);
                update_minmax(min, max, b);
                update_minmax(min, max, c);
            }

            std::shared_ptr<Triangle> triangle = std::make_shared<Triangle>(
                a, b, c, std::make_shared<Diffuse>(std::make_shared<ColorTexture>(
                    color
                ))
            );
            scene.add(triangle);
        }
    }
    std::cout << "min: " << min.a << " " << min.b << " " << min.c << std::endl;
    std::cout << "max: " << max.a << " " << max.b << " " << max.c << std::endl;

    #else
    std::cerr << "warn: not built with assimp. model " << path << " will be skipped" << std::endl;
    #endif
}