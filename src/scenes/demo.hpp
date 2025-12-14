#pragma once

#include <obj/scene.hpp>
#include <camera.hpp>
#include <obj/triangle.hpp>
#include <obj/quad.hpp>
#include <obj/sphere.hpp>
#include <obj/model.hpp>
#include <obj/material.hpp>
#include <obj/texture.hpp>
#include <math/vec3.hpp>

#include <iostream>

void demo(Scene &scene, std::vector<std::shared_ptr<Object>> &lights, CameraParams &params) {
    ModelLoader loader(scene);

    scene.add(std::make_shared<Quad>(
        Vec3(-10, 0, -10),
        Vec3(20, 0, 0),
        Vec3(0, 0, 20),
        std::make_shared<Diffuse>(std::make_shared<ColorTexture>(0.5, 0.5, 0.5))
    ));

    auto light = std::make_shared<Quad>(
        Vec3(-1, 0.01, -1),
        Vec3(2, 0.01, 0),
        Vec3(0, 0.01, 2),
        std::make_shared<DiffuseLight>(std::make_shared<ColorTexture>(3, 3, 3)
    ));
    scene.add(light);
    lights.push_back(light);

    scene.add(std::make_shared<Sphere>(
        Vec3(-3, 1, 0),
        1,
        std::make_shared<Diffuse>(std::make_shared<ColorTexture>(0, 0, 1)
    )));

    scene.add(std::make_shared<Sphere>(
        Vec3(3, 1, 0),
        1,
        std::make_shared<Diffuse>(std::make_shared<ColorTexture>(1, 0, 0)
    )));
    
    params.background = std::make_shared<ColorTexture>(Color::black);
    params.fov = 50;
    params.vup = Vec3(0, 1, 0);
    params.focus_distance = 25;
    params.defocus_angle = 0;
    params.look_from = Vec3(0, 5, 5);
    params.look_at = Vec3(0, 0, 0);
    return;
}