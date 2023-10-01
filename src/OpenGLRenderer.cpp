#include "OpenGLRenderer.h"

using namespace OdisEngine;

OpenGLRenderer::OpenGLRenderer(Window& window)
{
	glViewport(0, 0, window.get_window_width(), window.get_window_height());
	window.set_window_size_callback(std::bind(&OpenGLRenderer::window_size_callback, this, std::placeholders::_1, std::placeholders::_2));
}

void OpenGLRenderer::window_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}
