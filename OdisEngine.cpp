#include <iostream>
#include <filesystem>

//WINDOW AND GRAPHICS

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "include/OdisEngine.h"

int main()
{
    OdisEngine::Window window(800, 600, "OdisEngine", false);
    OdisEngine::Renderer renderer(window);
    OdisEngine::Input input;

    while (!window.shouldClose()) {
        input.pollInputs();
        

        renderer.clear();
        renderer.swapBuffers();
    }

    renderer.termimate();
    window.terminate();
    return 0;
}
