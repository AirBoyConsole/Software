#include "renderer.hpp"

namespace airboy 
{
    Renderer::Renderer(Display &display, Vector2i camera_size) 
    {
        camera = new Camera(camera_size);
    }

    Renderer::~Renderer() {}
    
}
