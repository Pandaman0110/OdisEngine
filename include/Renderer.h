#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include "OdisMath.h"

#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "ShapeRenderer.h"

#include "Color.h"
#include "Shapes.h"

namespace OdisEngine
{
	class Renderer
	{
	private:
		std::unique_ptr<SpriteRenderer> sprite_renderer;
		std::unique_ptr<TextRenderer> text_renderer;
		std::unique_ptr<ShapeRenderer> shape_renderer;

		void window_size_callback(int w, int h);

	public:
		Renderer(Window&, ResourceManager&);

		template <ColorTypeRGB T>
		void clear(T color = {1.0f, 1.0f, 1.0f}, float alpha = 1.0f)
		{
			glClearColor(color.r, color.g, color.b, alpha);
			glClear(GL_COLOR_BUFFER_BIT);
		};
		inline void clear(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
		{
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT);
		};

		template <ColorTypeRGB T>
		void draw_rect(ivec2 pos, ivec2 size, T color, float rotation = 0.0f, float alpha = 1.0f)
		{
			shape_renderer->draw_rect(pos, size, rotation, color, alpha);
		}

		template <RectangleType T1, ColorTypeRGB T2>
		void draw_rect(T1 rect, T2 color, float rotation = 0.0f, float alpha = 1.0f)
		{
			shape_renderer->draw_rect(std::move(ivec2{ rect.x, rect.y }), std::move(ivec2{ rect.w, rect.h }), rotation, color, alpha);
		}

		//texture drawing
		void draw_texture(Texture2D&, vec2 = { 0, 0 }, float  = 0.0f);

		//fonts and text
		void draw_text(std::string text, vec2 pos = {0.0f, 0.0f}, float scale = 1.0f, Color color = {0.0f, 0.0f, 0.0f});
		void set_font(Font&);

		void draw();
	};

}


#endif
