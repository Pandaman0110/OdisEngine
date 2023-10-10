#include "OpenGLRenderer2D.h"

using namespace OdisEngine;

OpenGLRenderer2D::OpenGLRenderer2D(Window& window, ResourceManager& resource_manager) : resource_manager(&resource_manager)
{
	window.set_window_size_callback(std::bind(&OpenGLRenderer2D::window_size_callback, this, std::placeholders::_1, std::placeholders::_2));
	glViewport(0, 0, window.get_window_width(), window.get_window_height());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mat4 projection = ortho(0.0f, static_cast<float>(window.get_window_width()), static_cast<float>(window.get_window_height()), 0.0f, -1.0f, 1.0f);

    this->resource_manager->load_shader("sprite.vert", "sprite.frag", nullptr, "sprite");
    
    this->resource_manager->get_shader("sprite").use().set_integer("image", 0);
    this->resource_manager->get_shader("sprite").set_matrix4("projection", projection);

    this->resource_manager->load_texture("tree.png", true, "tree");
    // set render-specific controls
    //Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    // load textures
    //ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
}

void OpenGLRenderer2D::draw_texture(Texture2D& texture, vec2 position)
{

}

void OpenGLRenderer2D::draw_texture(Texture2D& texture, vec2 position, float rotation)
{
    
}

void OpenGLRenderer2D::window_size_callback(int width, int height)
{
	glViewport(0, 0, width, height);
}
