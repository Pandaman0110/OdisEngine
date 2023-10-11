#include <iostream>
#include <filesystem>

//WINDOW AND GRAPHICS

#include "include/OdisEngine.h"

using namespace OdisEngine;

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

int cumdump() { return 9; }

int main()
{
    Window window(800, 600, "OdisEngine", false, RenderAPI::OpenGL);
    ResourceManager resource_manager;

    OpenGLRenderer2D renderer(window, resource_manager);
    //Renderer renderer = Renderer(window);
    Input input(window);
    
    //Script script;

    double delta_time = 0.0f;
    double last_frame_time = 0.0f;
    double current_frame_time = 0.0f;

    ImGui::CreateContext();

    ImGui_ImplGlfw_InitForOpenGL(window.get_window_handle(), true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();

    while (!window.should_close()) 
    {
        current_frame_time = glfwGetTime();
        delta_time = current_frame_time - last_frame_time;
        last_frame_time = current_frame_time;

        if (input.is_key_pressed(Key::KEY_ESCAPE))
            break;

        renderer.render();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)

        input.poll_inputs();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.end_frame();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

   // renderer.termimate();
    window.terminate();
    return 0;
}
