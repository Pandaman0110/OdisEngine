#include <iostream>
#include <filesystem>
#include <tuple>

//WINDOW AND GRAPHICS

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <entt.hpp>

#include "OdisEngine.h"
#include "OdisMath.h"

using namespace OdisEngine;

/*
void cum_system(Transform2D transform) 
{
    std::cout << transform << "\n"; 
};
*/

int main()
{
    Window window(1920, 1080, "OdisEngine", true, RenderAPI::OpenGL);
    ResourceManager resource_manager;

    Renderer renderer(window, resource_manager);
    Input input(window);

    double delta_time = 0.0f;
    double last_frame_time = 0.0f;
    double current_frame_time = 0.0f;

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window.get_window_handle(), true);
    ImGui_ImplOpenGL3_Init();

    /*
    World world;

    auto entity_0 = world.create_entity();
    auto entity_1 = world.create_entity();

    world.assign<Transform2D>(entity_0);
    
    world.update_system<Transform2D>(std::function<void(Transform2D)>(cum_system));
    */

 
    while (!window.should_close()) 
    {
        current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        input.poll_inputs();
        window.poll();
        

        if (input.is_key_pressed(Key::KEY_ESCAPE))
            break;

        //renderer.render(world);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        //ImGui::Begin("Demo");
        //if (ImGui::Button("Cum Button"))
        //    std::cout << "cum" << std::endl;
        //ImGui::End();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swap_buffers();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    window.terminate();
    return 0;
}
