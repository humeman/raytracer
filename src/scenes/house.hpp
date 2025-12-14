#pragma once

#include <obj/scene.hpp>
#include <camera.hpp>
#include <obj/quad.hpp>
#include <obj/sphere.hpp>
#include <obj/model.hpp>
#include <obj/material.hpp>
#include <obj/texture.hpp>
#include <math/vec3.hpp>

#include <iostream>

void make_box(Scene &scene, const Vec3 &a, const Vec3 &b, std::shared_ptr<Material> material, std::vector<std::shared_ptr<Object>> *lights = nullptr) {
    Vec3 min(std::fmin(a.a, b.a), std::fmin(a.b, b.b), std::fmin(a.c, b.c));
    Vec3 max(std::fmax(a.a, b.a), std::fmax(a.b, b.b), std::fmax(a.c, b.c));
    Vec3 x(max.a - min.a, 0, 0);
    Vec3 y(0, max.b - min.b, 0);
    Vec3 z(0, 0, max.c - min.c);

    
    auto s1 = std::make_shared<Quad>(Vec3(min.a, min.b, max.c), x, y, material);
    auto s2 = std::make_shared<Quad>(Vec3(max.a, min.b, max.c), -z, y, material);
    auto s3 = std::make_shared<Quad>(Vec3(max.a, min.b, min.c), -x, y, material);
    auto s4 = std::make_shared<Quad>(Vec3(min.a, min.b, min.c), z, y, material);
    auto s5 = std::make_shared<Quad>(Vec3(min.a, max.b, max.c), x, -z, material);
    auto s6 = std::make_shared<Quad>(Vec3(min.a, min.b, min.c), x, z, material);
    scene.add(s1);
    scene.add(s2);
    scene.add(s3);
    scene.add(s4);
    scene.add(s5);
    scene.add(s6);
    if (lights != nullptr) {
        lights->push_back(s1);
        lights->push_back(s2);
        lights->push_back(s3);
        lights->push_back(s4);
        lights->push_back(s5);
        lights->push_back(s6);
    }
}

std::shared_ptr<Quad> quad(double x0, double y0, double z0, double x1, double y1, double z1, std::shared_ptr<Material> material) {
    std::vector<Vec3> vecs;
    if (!DOUBLE_EQ(x0, x1))
        vecs.push_back(Vec3(x1 - x0, 0, 0));
    if (!DOUBLE_EQ(y0, y1))
        vecs.push_back(Vec3(0, y1 - y0, 0));
    if (!DOUBLE_EQ(z0, z1))
        vecs.push_back(Vec3(0, 0, z1 - z0));

    if (vecs.size() != 2)
        throw EXC("quads need to vary in exactly 2 dimensions");
    
    Vec3 origin(x0, y0, z0);
    return std::make_shared<Quad>(origin, vecs[0], vecs[1], material);
}

#define DIFFUSE_COLOR(col) std::make_shared<Diffuse>(std::make_shared<ColorTexture>(col))
#define RGB(a, b, c) Color(a / 255.0, b / 255.0, c / 255.0)

void house_md(Scene &scene, std::vector<std::shared_ptr<Object>> &lights, CameraParams &params) {
    ModelLoader loader(scene);
    auto wall_color = DIFFUSE_COLOR(RGB(70, 64, 48));
    auto ceiling_color = DIFFUSE_COLOR(RGB(70, 64, 48) * 0.7);
    auto floor_color = DIFFUSE_COLOR(RGB(89, 81, 72));
    auto doorway_trim = DIFFUSE_COLOR(Color(0.9, 0.9, 0.9));
    auto blinds = DIFFUSE_COLOR(Color(0.99, 0.97, 0.95));
    auto bookshelf = DIFFUSE_COLOR(RGB(222, 222, 222));
    auto blinds_trim = DIFFUSE_COLOR(RGB(78, 80, 87));
    auto wall_panels = DIFFUSE_COLOR(RGB(138, 56, 15));
    auto frame = std::make_shared<Metal>(std::make_shared<ColorTexture>(0.7, 0.7, 0.7), 0.4);
    // Floor
    scene.add(quad(0, 0, 0, 40, 0, 20, floor_color));
    // Ceiling
    scene.add(quad(0, 16, -20, 40, 16, 20, ceiling_color));
    // L Wall
    scene.add(quad(0, 0, 20, 40, 16, 20, wall_color));
    // R Wall
    scene.add(quad(0, 12, 0, 40, 16, 0, wall_color));
    scene.add(quad(0, 0, 0, 20, 12, 0, wall_color));
    scene.add(quad(24, 0, 0, 35, 12, 0, wall_color));
    scene.add(quad(39, 0, 0, 40, 12, 0, wall_color));
    // R Doorway 1 (x=19...24)
    scene.add(quad(24, 12, 0, 24, 0, -1, doorway_trim));
    scene.add(quad(20, 12, 0, 24, 12, -1, doorway_trim));
    scene.add(quad(19, 0, 0, 40, 0, -19, floor_color));
    // R Doorway 2 (x=34...39)
    scene.add(quad(39, 12, -1, 39, 0, 0, doorway_trim));
    scene.add(quad(34, 12, 0, 39, 12, -1, doorway_trim));
    
    // Doorway 1 door
    auto glass = std::make_shared<Dielectric>(0.7, Color(0.6, 0.6, 0.6));
    make_box(scene, Vec3(23.8, 0.2, -0.5), Vec3(24, 11.8, 4.5), glass);
    // Handle
    make_box(scene, Vec3(23.7, 5, 3.9), Vec3(24.1, 7, 4.25), frame);

    // Back Wall
    scene.add(quad(40, 15, 0, 40, 16, 20, wall_color));
    scene.add(quad(40, 0, 0, 40, 0.5, 20, doorway_trim));
    scene.add(quad(40, 0, 0, 40, 16, -20, wall_color));
    scene.add(quad(40, 0.5, 9.5, 40, 15, 10.5, doorway_trim));
    scene.add(quad(40, 6.5, 0, 40, 8, 20, doorway_trim));
    // auto outside = quad(40.5, -2, -2, 40.5, 18, 22, std::make_shared<DiffuseLight>(std::make_shared<ColorTexture>(Color(1, 0.95, 0.9))));
    // lights.push_back(outside);
    // scene.add(outside);
    // Blinds
    for (int z = 1; z <= 19; z++) {
        scene.add(
            std::make_shared<Quad>(
                Vec3(40, 0.5, z),
                Vec3(-0.7, 0, -0.6),
                Vec3(0, 14.5, 0),
                blinds
            )
        );
    }
    make_box(scene, Vec3(39.2, 16, 0), Vec3(40, 17, 20), blinds_trim);

    // Ceiling lights
    auto light_color = std::make_shared<DiffuseLight>(std::make_shared<ColorTexture>(RGB(255, 230, 179) * 0.75));
    make_box(scene, Vec3(3, 15.75, 2), Vec3(7, 16, 18), light_color, &lights);
    make_box(scene, Vec3(18, 15.75, 2), Vec3(22, 16, 18), light_color, &lights);
    make_box(scene, Vec3(33, 15.75, 2), Vec3(37, 16, 18), light_color, &lights);

    // Desk
    auto desk_glass = std::make_shared<Dielectric>(0.7, Color(0.6, 0.6, 0.8));
    make_box(scene, Vec3(24, 4.5, 15.9), Vec3(34, 4.75, 19.9), desk_glass);
    make_box(scene, Vec3(24, 4.5, 8.9), Vec3(29, 4.75, 15.9), desk_glass);
    loader.load("assets/computer.glb", Vec3(30 + 1, 4.75, 17), 2.35);
    
    make_box(scene, Vec3(24, 4.2, 19.9), Vec3(34, 4.5, 19.6), frame);
    make_box(scene, Vec3(24, 4.2, 15.9), Vec3(34, 4.5, 16.2), frame);
    make_box(scene, Vec3(24, 4.2, 8.9), Vec3(24.3, 4.5, 19.9), frame);
    make_box(scene, Vec3(33.7, 4.2, 19.9), Vec3(34, 4.5, 15.9), frame);
    make_box(scene, Vec3(24, 4.2, 8.9), Vec3(29, 4.5, 9.2), frame);
    make_box(scene, Vec3(28.7, 4.2, 8.9), Vec3(29, 4.5, 15.9), frame);
    make_box(scene, Vec3(24, 0, 8.9), Vec3(24.3, 4.2, 9.2), frame);
    make_box(scene, Vec3(24, 0, 19.9), Vec3(24.3, 4.2, 19.6), frame);
    make_box(scene, Vec3(28.7, 0, 8.9), Vec3(29, 4.2, 9.2), frame);
    make_box(scene, Vec3(33.7, 0, 19.9), Vec3(34, 4.2, 19.6), frame);
    make_box(scene, Vec3(33.7, 0, 15.9), Vec3(34, 4.2, 16.2), frame);

    // Random desk stuff
    loader.load("assets/books.glb", Vec3(24 + 2, 4.75, 10 + 1), 0.75);
    loader.load("assets/desk_lamp.glb", Vec3(26.5, 4.75, 15.3), 1.25);
    loader.load("assets/coffee_cup.glb", Vec3(27.5, 5.25, 10.7), 0.25);
    loader.load("assets/book_stack.glb", Vec3(26.5, 4.75, 19.5), 0.25);

    // Rear bookshelf
    make_box(scene, Vec3(36, 0, 7.7), Vec3(39, 0.3, 8), bookshelf);
    make_box(scene, Vec3(36, 0, 12), Vec3(39, 0.3, 12.3), bookshelf);
    make_box(scene, Vec3(36, 0.3, 6), Vec3(39, 0.6, 14), bookshelf);
    make_box(scene, Vec3(36, 2, 6), Vec3(39, 2.1, 14), bookshelf);
    make_box(scene, Vec3(36, 3.5, 6), Vec3(39, 3.8, 14), bookshelf);
    make_box(scene, Vec3(36, 0.6, 6), Vec3(39, 3.5, 6.3), bookshelf);
    make_box(scene, Vec3(36, 0.6, 13.7), Vec3(39, 3.5, 14), bookshelf);
    scene.add(quad(39, 0.6, 6.3, 39, 3.5, 13.7, bookshelf));

    // Rear bookshelf stuff
    loader.load("assets/skull.glb", Vec3(37.5, 4.03, 7.5), 0.02, Vec3(0, 1, 0), 270);
    loader.load("assets/book_row.glb", Vec3(37, 2.1, 8.5), 0.03);
    loader.load("assets/book_row.glb", Vec3(37.1, 0.6, 11.5), 0.03);
    loader.load("assets/globe.glb", Vec3(30.2, 4.55, 13.5), 1);

    // Wall panels
    for (int x = 0; x < 40; x += 10) {
        for (int y = 0; y < 12; y += 4.25) {
            double x0 = x + 0.25;
            double y0 = y + 0.25;
            double x1 = x + 10 - 0.25;
            double y1 = y + 4.25 - 0.25;
            scene.add(quad(x0, y0, 19.9, x1, y1, 19.9, wall_panels));
        }
    }

    // Left bookshelf
    auto l_bookshelf = std::make_shared<Metal>(std::make_shared<ColorTexture>(0.7, 0.7, 0.7), 0.3);
    make_box(scene, Vec3(27, 0, 0), Vec3(32, 0.3, 4), l_bookshelf);
    make_box(scene, Vec3(27, 3, 0), Vec3(32, 3.3, 4), l_bookshelf);
    make_box(scene, Vec3(27, 6, 0), Vec3(32, 6.3, 4), l_bookshelf);
    make_box(scene, Vec3(27, 0.3, 0), Vec3(27.3, 6, 4), l_bookshelf);
    make_box(scene, Vec3(31.7, 0.3, 0), Vec3(32, 6, 4), l_bookshelf);

    // Light panel
    make_box(scene, Vec3(26, 7, 0), Vec3(33, 7.3, 0.75), bookshelf);
    make_box(scene, Vec3(26, 11, 0), Vec3(33, 11.3, 0.75), bookshelf);
    make_box(scene, Vec3(26, 7.3, 0), Vec3(26.3, 11, 0.75), bookshelf);
    make_box(scene, Vec3(32.7, 7.3, 0), Vec3(33, 11, 0.75), bookshelf);
    auto light_panel = std::make_shared<DiffuseLight>(std::make_shared<ColorTexture>(Color::white));
    scene.add(quad(26.3, 7.3, 0.4, 32.7, 11, 0.4, light_panel));

    // Chairs
    loader.load("assets/chair.glb", Vec3(32.5, 0, 13.5), 6, Vec3(0, 1, 0), 270);
    loader.load("assets/visitor_chair.glb", Vec3(21 + 47, 0, 9 + 30), 0.13, Vec3(0, 1, 0), 125);
    loader.load("assets/visitor_chair.glb", Vec3(21 - 13, 0, 9 + 53), 0.13, Vec3(0, 1, 0), 45);


    params.background = std::make_shared<ColorTexture>(RGB(199, 251, 255));
    params.fov = 50;
    params.vup = Vec3(0, 1, 0);
    params.focus_distance = 25;
    params.defocus_angle = 0.2;
    params.look_from = Vec3(8, 8, 8);
    params.look_at = Vec3(30, 7, 10);
    // params.look_from = Vec3(31, 7, 10);
    // params.look_at = Vec3(36, 5, 10);
    return;
}