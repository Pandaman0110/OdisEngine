#include "Renderer.h"

#include <memory>
#include <concepts>
#include <type_traits>

#include "glad/gl.h"

using namespace OdisEngine;

Renderer::Renderer(Window& window, ResourceManager& resource_manager)
{
	//todo clean this up
	window.set_window_size_callback(std::bind(&Renderer::window_size_callback, this, std::placeholders::_1, std::placeholders::_2));
	glViewport(0, 0, window.get_window_width(), window.get_window_height());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mat4 projection = ortho(0.0f, static_cast<float>(window.get_window_width()), static_cast<float>(window.get_window_height()), 0.0f, -1.0f, 1.0f);
	//setup sprite renderer
	sprite_renderer = std::make_unique<SpriteRenderer>(resource_manager.load_shader("sprite.vert", "sprite.frag", "", "sprite"));
	auto& sprite_shader = resource_manager.get_shader("sprite");
	sprite_shader.use().set_integer("text", 0);
	sprite_shader.set_matrix4("projection", projection);

	text_renderer = std::make_unique<TextRenderer>(resource_manager.load_shader("text.vert", "text.frag", "", "text"));
	auto& text_shader = resource_manager.get_shader("text");
	text_shader.use().set_integer("text", 0);
	text_shader.set_matrix4("projection", projection);

	shape_renderer = std::make_unique<ShapeRenderer>(resource_manager.load_shader("shape.vert", "shape.frag", "", "shape"));
	auto& shape_shader = resource_manager.get_shader("shape");
	shape_shader.use().set_matrix4("projection", projection);
}



void Renderer::draw_texture(Texture2D& texture, vec2 position, float rotation)
{
	sprite_renderer->draw_texture(texture, position, rotation);
}


void Renderer::set_font(Font& font)
{
	text_renderer->load_font(font);
}

void Renderer::draw_text(std::string text, vec2 pos, float scale, Color color)
{
	text_renderer->draw_text(text, pos, scale, color);
}



void Renderer::draw()
{
	sprite_renderer->draw();
	text_renderer->draw();
}



void Renderer::window_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}
