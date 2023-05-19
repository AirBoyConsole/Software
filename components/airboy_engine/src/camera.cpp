#include "camera.hpp"

namespace airboy 
{
    Camera::Camera(Vector2i camera_size, bool smooth_move, float move_speed) 
    {
        position = Vector2f::ZERO;
        target = Vector2f::ZERO;
        offset = Vector2i::ZERO;
        size = camera_size;

        smooth = smooth_move;
        step = move_speed;

        limit_max_x = 100000;
        limit_max_y = 100000;
        limit_min_x = 100000;
        limit_min_y = 100000;
    }

    Camera::Camera(Vector2i camera_size) 
    {
        position = Vector2f::ZERO;
        target = Vector2f::ZERO;
        offset = Vector2i::ZERO;
        size = camera_size;

        smooth = true;
        step = 0.2;

        limit_max_x = 100000;
        limit_max_y = 100000;
        limit_min_x = 100000;
        limit_min_y = 100000;
    }

    Camera::~Camera() {}

    void Camera::update()
    {
        if (smooth)
        {
            position.x = std::lerp(position.x, target.x, step);
            position.y = std::lerp(position.y, target.y, step);
        }else{
            
            position = target;
        }
    }

    void Camera::move(Vector2f value)
    {
        Vector2f new_target = position + value;

        
        //if ((new_target.x + this->size.x > limit_max_x) || (new_target.x < limit_min_x) || (new_target.y + this->size.y > limit_max_y) || (new_target.y < limit_min_y)) return;

        target = new_target;
    }

    Vector2i Camera::get_size()
    {
        return this->size;
    }
    
}
