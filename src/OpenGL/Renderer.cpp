#include "Renderer.h"

#include "glad/gl.h"

using namespace OdisEngine;

Renderer::Renderer(Window& window, ResourceManager& resource_manager) : resource_manager(&resource_manager)
{
	
	window.set_window_size_callback(std::bind(&Renderer::window_size_callback, this, std::placeholders::_1, std::placeholders::_2));
	glViewport(0, 0, window.get_window_width(), window.get_window_height());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mat4 projection = ortho(0.0f, static_cast<float>(window.get_window_width()), static_cast<float>(window.get_window_height()), 0.0f, -1.0f, 1.0f);

	sprite_renderer = new SpriteRenderer(this->resource_manager->load_shader("sprite.vert", "sprite.frag", nullptr, "sprite"));
    
    this->resource_manager->get_shader("sprite").use().set_integer("image", 0);
    this->resource_manager->get_shader("sprite").set_matrix4("projection", projection);

	/*
    auto &text = this->resource_manager->load_texture("catgreyidle.png", true, "cat");
	*/
}

void Renderer::clear(Color color)
{
	glClearColor(color.r, color.g, color.b, color.a);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::draw_texture(Texture2D& texture, vec2 position, float rotation)
{
	sprite_renderer->draw_texture(texture, position, rotation);
}

void Renderer::draw()
{
	sprite_renderer->draw();
}



void Renderer::window_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}
