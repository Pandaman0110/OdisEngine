#include <iostream>
#include <filesystem>

//WINDOW AND GRAPHICS


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "include/OdisEngine.h"

using namespace OdisEngine;

int main()
{
    Window window = Window(800, 600, "OdisEngine", false, RenderAPI::OpenGL);
    OpenGLRenderer renderer = OpenGLRenderer(window);
    //Renderer renderer = Renderer(window);
    Input input = Input(window);

    while (!window.should_close()) {
        input.poll_inputs();
       
        window.swap_buffers();
    }

   // renderer.termimate();
    window.terminate();
    return 0;
}
