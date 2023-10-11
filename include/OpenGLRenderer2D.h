#ifndef OPENGLRENDERER2D_H
#define OPENGLRENDERER2D_H

#include "Window.h"
#include "OdisMath.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"

namespace OdisEngine
{
	class OpenGLRenderer2D
	{
	public:
		OpenGLRenderer2D(Window& window, ResourceManager& resource_manager);

		void render();

	private:
		ResourceManager* resource_manager;

		SpriteRenderer* sprite_renderer;

		void window_size_callback(int width, int height);
	};

}


#endif
