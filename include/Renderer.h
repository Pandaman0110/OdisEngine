#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include "OdisMath.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "Color.h"

namespace OdisEngine
{
	class Renderer
	{
	private:
		ResourceManager* resource_manager;
		SpriteRenderer* sprite_renderer;

		void window_size_callback(int width, int height);

	public:
		Renderer(Window& window, ResourceManager& resource_manager);

		void clear(Color color);
		void draw_texture(Texture2D& texture, vec2 position = { 0, 0 }, float rotation = 0.0f);

		void draw();
	};

}


#endif
