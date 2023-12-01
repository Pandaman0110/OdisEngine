#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <concepts>
#include <type_traits>
#include <iostream>
#include <fstream>
#include <sstream>

#include "glad/gl.h"

#include "Window.h"
#include "ResourceManager.h"
#include "SpriteRenderer.h"
#include "TextRenderer.h"
#include "ShapeRenderer.h"
#include "Log.h"

#include "utility/OdisMath.h"
#include "Color.h"
#include "Camera.h"

namespace OdisEngine
{
	template <VectorType T = glm::ivec2>
	T calculate_res_scale(T game_resolution, T target_resolution)
	{
		return T { target_resolution.x / game_resolution.x, target_resolution.y / game_resolution.y };
	}

	template <IntVectorType T = glm::ivec2>
	T calculate_resolution(T game_resolution, T target_resolution)
	{
		T scale = this->calculate_scale(game_resolution, target_resolution);
		return T{ game_resolution.x * scale.x, game_resolution.y * scale.y };
	}

	template <IntVectorType T = glm::ivec2>
	T calculate_offset(T game_resolution, T screen_size) 
	{
		return T{ (screen_size.x - game_resolution.x) / 2, (screen_size.y - game_resolution.y) / 2 };
	}

	enum class ScaleMode
	{
		normal,
		integer
	};

	class Renderer
	{
	private:
		std::unique_ptr<SpriteRenderer> sprite_renderer;
		std::unique_ptr<TextRenderer> text_renderer;
		std::unique_ptr<ShapeRenderer> shape_renderer;

		Camera2D camera;
		glm::ivec2 game_resolution;
		ScaleMode scale_mode;


		void window_size_callback(int width, int height)
		{
			
		}

		void set_viewport(int offset_x, int offset_y, int width, int height)
		{
			glViewport(offset_x, offset_y, width, height);
			std::cout << offset_x << " " << offset_y << " " << width << " " << height << std::endl;
		}

		template <IntVectorType T>
		void set_viewport(T offset, T size)
		{
			set_viewport(offset.x, offset.y, size.x, size.y);
		}

		void setup(Window* window)
		{
			window->set_window_size_callback(std::bind(&Renderer::window_size_callback, this, std::placeholders::_1, std::placeholders::_2));

			auto c = logger->create("OdisEngine");
			int version;
			std::string ver;

			version = gladLoadGL(window->get_proc_address());
			ver = std::to_string(GLAD_VERSION_MAJOR(version)) + "." + std::to_string(GLAD_VERSION_MINOR(version));

			if (version == 0)
			{
				c->log(LogLevel::fatal, "Failed to initialize OpenGL context");
			}

			c->log(LogLevel::info, "Loaded OpenGL version", ver);

			int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
			if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
			{
				c->log(LogLevel::info, "Debug context succesfully initialized");

				glEnable(GL_DEBUG_OUTPUT);
				glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); 
				glDebugMessageCallback(opengl_debug_callback, nullptr);
				glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
			}
			else
			{
				c->log(LogLevel::info, "Debug context not initialized");
			}
		}

		static void opengl_debug_callback(GLenum source,
			GLenum type,
			unsigned int id,
			GLenum severity,
			GLsizei length,
			const char* message,
			const void* userParam)
		{
			auto c = logger->get("OdisEngine");

			// ignore non-significant error/warning codes
			if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

			std::stringstream source_string, type_string;

			switch (source)
			{
			case GL_DEBUG_SOURCE_API:             source_string << "API"; break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   source_string << "Window System"; break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER: source_string << "Shader Compiler"; break;
			case GL_DEBUG_SOURCE_THIRD_PARTY:     source_string << "Third Party"; break;
			case GL_DEBUG_SOURCE_APPLICATION:     source_string << "Application"; break;
			case GL_DEBUG_SOURCE_OTHER:           source_string << "Other"; break;
			}

			switch (type)
			{
			case GL_DEBUG_TYPE_ERROR:               type_string << "Error"; break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: type_string << "Deprecated Behaviour"; break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  type_string << "Undefined Behaviour"; break;
			case GL_DEBUG_TYPE_PORTABILITY:         type_string << "Portability"; break;
			case GL_DEBUG_TYPE_PERFORMANCE:         type_string << "Performance"; break;
			case GL_DEBUG_TYPE_MARKER:              type_string << "Marker"; break;
			case GL_DEBUG_TYPE_PUSH_GROUP:          type_string << "Push Group"; break;
			case GL_DEBUG_TYPE_POP_GROUP:           type_string << "Pop Group"; break;
			case GL_DEBUG_TYPE_OTHER:               type_string << "Other"; break;
			}

			LogLevel level = LogLevel::debug;

			switch (severity)
			{
			case GL_DEBUG_SEVERITY_HIGH:         level = LogLevel::fatal; break;
			case GL_DEBUG_SEVERITY_MEDIUM:       level = LogLevel::error; break;
			case GL_DEBUG_SEVERITY_LOW:          level = LogLevel::warning; break;
			case GL_DEBUG_SEVERITY_NOTIFICATION: level = LogLevel::info; break;
			}
			
			c->logf(level, "Source: {}, Type: {}, ID: ({}), OpenGL Debug Message: \n{}", source_string.str(), type_string.str(), id, message);
		}
	public:
		Renderer(Window* window, ResourceManager* resource_manager, ScaleMode scale_mode = ScaleMode::normal) : scale_mode(scale_mode)
		{
			this->setup(window);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			//glm::ivec2 resolution = calculate_resolution( ,window.get_window_size())

			glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(window->get_window_width()), static_cast<float>(window->get_window_height()), 0.0f, -1.0f, 1.0f);

			sprite_renderer = std::make_unique<SpriteRenderer>(resource_manager->load_shader("sprite.vert", "sprite.frag", "", "sprite"));
			auto& sprite_shader = resource_manager->get_shader("sprite");
			sprite_shader.use().set_integer("text", 0);
			sprite_shader.set_matrix4("projection", projection);

			text_renderer = std::make_unique<TextRenderer>(resource_manager->load_shader("text.vert", "text.frag", "", "text"));
			auto& text_shader = resource_manager->get_shader("text");
			text_shader.use().set_integer("text", 0);
			text_shader.set_matrix4("projection", projection);

			shape_renderer = std::make_unique<ShapeRenderer>(resource_manager->load_shader("shape.vert", "shape.frag", "", "shape"));
			auto& shape_shader = resource_manager->get_shader("shape");
			shape_shader.use().set_matrix4("projection", projection);
		}

		template <IntVectorType T = glm::ivec2>
		void set_resolution(T game_resolution = { 0, 0 })
		{
			this->game_resolution.x = game_resolution.x;
			this->game_resolution.y = game_resolution.y;
		}
		
		void set_resolution(int width = 0, int height = 0)
		{
			this->game_resolution.x = width;
			this->game_resolution.y = height;
		}


		/** \overload */
		void clear()
		{
			clear(1.0f, 1.0f, 1.0f, 1.0f);
		}

		/** \overload */
		void clear(float r, float g, float b)
		{
			//????????????
			clear(r, g, b, 1.0f);
		}

		/// Clears the screen with a color, defaults to white
		/** All values range from 0 to 1
		 * \param r float value representing the red component of the color
		 * \param g float value representing the green component of the color
		 * \param b float value representing the blue component of the color
		 * \param a float value representing the alpha component of the color
		 *
		 */
		void clear(float r, float g, float b, float a)
		{
			//disable z buf for 2D
			glDisable(GL_DEPTH_TEST);
			glClearColor(r, g, b, a);
			glClear(GL_COLOR_BUFFER_BIT);
		}

		/** \overload */
		template <ColorTypeRGB T>
		void clear(T color)
		{
			clear(color.r, color.b, color.g, 1.0f);
		}

		/**
		 * \overload
		 * 
		 * \tparam color value fulfilling the concept OdisEngine::ColorTypeRGB
		 * \param a float value representing the alpha component of the color
		 */
		template <ColorTypeRGB T>
		void clear(T color, float a)
		{
			clear(color.r, color.b, color.g, a);
		}

		/** \overload */
		template <IntVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_rect(T1 pos, T1 size, T2 color)
		{
			draw_rect(pos, size, color, 0.0f, 1.0f);
		}

		/** \overload */
		template <IntVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_rect(T1 pos, T1 size, T2 color, float rotation)
		{
			draw_rect(pos, size, color, rotation, 1.0f);
		}
		
		///Draws a rectangle
		/**
		 * \tparam pos value fulfilling the concept OdisEngine::IntVectorType, defaults to glm::ivec2
		 * \tparam size value fulfilling the concept OdisEngine::IntVectorType, defaults to glm::ivec2
		 * \tparam color value fulfilling the concept OdisEngine::ColorTypeRGB, defaults to glm::vec3. valid values range from 0 to 1
		 * \param rotation float value representing the rotation in degrees 
		 * \param alpha float value representing the alpha of the rectangle, valid values range from 0 to 1
		 */
		template <IntVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_rect(T1 pos, T1 size, T2 color, float rotation, float alpha)
		{
			shape_renderer->draw_rect(pos, size, color, rotation, alpha);
		}

		template <IntVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_circle(T1 pos, float radius, T2 color)
		{

		}

		template <IntVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_polygon(T1 pos, float radius, size_t num_sides, T2 color)
		{
			this->draw_polygon(pos, radius, num_sides, color, 0.0f);
		}

		template <IntVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_polygon(T1 pos, float radius, size_t num_sides, T2 color, float rotation)
		{
			shape_renderer->draw_polygon(pos, radius, num_sides, color, rotation);
		}

		/**
		 * .
		 * 
		 * \param texture Texture2D the texture to be drawn 
		 * \tparam pos value fulfilling the concept OdisEngine::IntVectorType, defaults to glm::ivec2
		 * \param rotation float value representing the rotation in degrees 
		 */
		template <IntVectorType T = glm::ivec2>
		void draw_texture(Texture& texture, T pos, float rotation = 0.0f)
		{
			sprite_renderer->draw_texture(texture, pos, rotation);
		}

		template <FloatVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_text(std::string text, T1 pos)
		{
			draw_text(text, pos, T2{1.0f, 1.0f, 1.0f}, 1.0f);
		}

		template <FloatVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_text(std::string text, T1 pos, T2 color)
		{
			draw_text(text, pos, color, 1.0f);
		}

		template <FloatVectorType T1 = glm::ivec2, ColorTypeRGB T2 = glm::vec3>
		void draw_text(std::string text, T1 pos, T2 color, float scale)
		{
			text_renderer->draw_text(text, pos, color, scale);
		}


		void draw()
		{
		}

		///Sets the active font to draw with void OdisEngine::Renderer::draw_text()
		/**
		 * \param font The Font object to set as the active font
		 */
		void set_font(Font& font)
		{
			text_renderer->load_font(font);
		}
	};

}


#endif
