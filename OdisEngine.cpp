#include <iostream>
#include <filesystem>

//WINDOW AND GRAPHICS


#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "include/OdisEngine.h"

using namespace OdisEngine;

int main()
{
    Window window(800, 600, "OdisEngine", false, RenderAPI::OpenGL);
    OpenGLRenderer renderer(window);
    //Renderer renderer = Renderer(window);
    Input input(window);

    while (!window.should_close()) {

        if (input.is_key_down(Key::KEY_D))
            std::cout << "CUM NUGGETS" << std::endl;
      
        input.poll_inputs();
        window.end_frame();
    }

   // renderer.termimate();
    window.terminate();
    return 0;
}
