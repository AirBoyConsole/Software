#pragma once

#include <iostream>

#include "vector2.hpp"
#include "math.hpp"
#include "renderer.hpp"

namespace airboy {

#define CAMERA_TAG "Camera"

#ifdef __cplusplus
extern "C" {
#endif

class Camera 
{
public:
    Camera(Vector2i camera_size, bool smooth_move, float move_speed);
    Camera(Vector2i camera_size);
    ~Camera();

    void update();
    void move(Vector2f value);
    Vector2i get_size();

    friend class Renderer;

private:
    Vector2f position;
    Vector2i size;
    Vector2f target;
    Vector2i offset;

    float step;
    bool smooth;

    int limit_max_x;
    int limit_min_x;
    int limit_max_y;
    int limit_min_y;
};

#ifdef __cplusplus
}
#endif
}

