#ifndef OPENGLRENDERER2D_H
#define OPENGLRENDERER2D_H

#include "Window.h"

#include "ResourceManager.h"
#include <OdisMath.h>

namespace OdisEngine
{
	class OpenGLRenderer2D
	{
	public:
		OpenGLRenderer2D(Window& window, ResourceManager& resource_manager);

		void draw_texture(Texture2D& texture, vec2 position);
		void draw_texture(Texture2D& texture, vec2 position, float rotation);
	private:
		ResourceManager* resource_manager;

		void window_size_callback(int width, int height);
	};

}


#endif
