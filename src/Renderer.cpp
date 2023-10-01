#include "Renderer.h"

#include <bx/bx.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>

#include <GLFW/glfw3.h>

#if BX_PLATFORM_LINUX
#define GLFW_EXPOSE_NATIVE_X11
#elif BX_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BX_PLATFORM_OSX
#define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <GLFW/glfw3native.h>

using namespace OdisEngine;

Renderer::Renderer(const Window& window)
{
    // Initialize bgfx using the native window handle and window resolution.
    bgfx::Init init;

#if BX_PLATFORM_LINUX || BX_PLATFORM_BSD
    init.platformData.ndt = glfwGetX11Display();
    init.platformData.nwh = (void*)(uintptr_t)glfwGetX11Window(window);
#elif BX_PLATFORM_OSX
    init.platformData.nwh = glfwGetCocoaWindow(window);
#elif BX_PLATFORM_WINDOWS
    init.platformData.nwh = glfwGetWin32Window(window.native_window);
#endif

    init.resolution.width = window.window_width;
    init.resolution.height = window.window_height;
    init.resolution.reset = BGFX_RESET_VSYNC;

    //automatically choose renderer type
    init.type = bgfx::RendererType::Count;

    if (!bgfx::init(init))
    {
        std::cout << "BGFX INIT FAILED" << std::endl;
        std::abort();
    }

    // Set view 0 to the same dimensions as the window and to clear the color buffer.
    clear_view = 0;
    bgfx::setViewClear(clear_view, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x223355FF, 1.0f, 0);
    bgfx::setViewRect(clear_view, 0, 0, window.window_width, window.window_height);
}

int Renderer::swapBuffers()
{
    return bgfx::frame();
}

void Renderer::clear()
{
    bgfx::touch(clear_view);
}

void Renderer::termimate()
{
    bgfx::shutdown();
}