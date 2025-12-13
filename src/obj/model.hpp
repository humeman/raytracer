#pragma once

#ifdef ASSIMP
#include <assimp/Importer.hpp>
#endif

#include <obj/scene.hpp>

class ModelLoader {
    private:
        Scene &scene;
        Color default_color = Color::black;
        #ifdef ASSIMP
        Assimp::Importer importer;
        #endif

    public:
        ModelLoader(Scene &scene) : scene(scene) {}

        void set_default_color(Color color);
        void load(
            const std::string &path, 
            const Vec3 &origin, 
            double scale= 1.0, 
            const Vec3 &rot_axis = Vec3::none,
            double rot_angle = 0);
};