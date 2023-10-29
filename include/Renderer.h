#ifndef RENDERER_H
#define RENDERER_H


#include "Window.h"
#include "OdisMath.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "Color.h"

namespace OdisEngine
{
	class Renderer
	{
	private:
		std::unique_ptr<SpriteRenderer> sprite_renderer;
		std::unique_ptr<TextRenderer> text_renderer;

		void window_size_callback(int w, int h);

	public:
		Renderer(Window&, ResourceManager&);

		template <ColorTypeRGBA T>
		void clear(T color)
		{
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT);
		};
		template <ColorTypeRGB T>
		void clear(T color)
		{
			glClearColor(color.r, color.g, color.b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		};
		inline void clear(float r, float g, float b, float a)
		{
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT);
		};
		inline void clear(float r, float g, float b)
		{
			glClearColor(r, g, b, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
		};


		//texture drawing
		void draw_texture(Texture2D&, vec2 = { 0, 0 }, float  = 0.0f);

		//fonts and text
		void draw_text(std::string text, vec2 pos = {0.0f, 0.0f}, float scale = 1.0f, Color color = {0.0f, 0.0f, 0.0f});
		void set_font(Font&);

		void draw();
	};

}


#endif
