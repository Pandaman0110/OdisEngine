#include <iostream>
#include <filesystem>

//WINDOW AND GRAPHICS

#include "include/OdisEngine.h"

using namespace OdisEngine;

int cumdump() { return 9; }

int main()
{
    Window window(1920, 1080, "OdisEngine", true, RenderAPI::OpenGL);
    ResourceManager resource_manager;

    OpenGLRenderer2D renderer(window, resource_manager);
    //Renderer renderer = Renderer(window);
    Input input(window);
    
    //Script script;

    double delta_time = 0.0f;
    double last_frame_time = 0.0f;
    double current_frame_time = 0.0f;

    while (!window.should_close()) 
    {
        current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        if (input.is_key_pressed(Key::KEY_ESCAPE))
            break;

        input.poll_inputs();
        window.end_frame();
    }

   // renderer.termimate();
    window.terminate();
    return 0;
}
