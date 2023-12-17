#ifndef ODISHGL_SHADER_H
#define ODISHGL_SHADER_H

#include <iostream>

#include <string>
#include <string_view>
#include <cstdint>
#include <array>
#include <algorithm>
#include <expected>
#include <span>

#include <glad/gl.h>

#include "OdisMath.h"

namespace OdisGL
{
	using namespace OdisMath;

	constexpr std::size_t error_log_size = 1024;

	enum class ShaderType
	{
		vertex = GL_VERTEX_SHADER,
		fragment = GL_FRAGMENT_SHADER,
		geometry = GL_GEOMETRY_SHADER,
		program = GL_PROGRAM
	};

	inline std::ostream& operator << (std::ostream& os, ShaderType type)
	{
		switch (type)
		{
			case ShaderType::vertex:
			{
				os << "Vertex Shader";
				break;
			}
			case ShaderType::fragment:
			{
				os << "Fragment Shader";
				break;
			}
			case ShaderType::geometry:
			{
				os << "Geometry Shader";
				break;
			}
			case ShaderType::program:
			{
				os << "Shader Program";
				break;
			}
		}

		return os;
	}


	class Shader
	{
	public:
		friend class ShaderProgram;

		Shader(ShaderType type, std::string_view shader_source)
		{
			this->id = glCreateShader(static_cast<GLenum>(type));

			auto data = shader_source.data();
			auto size = static_cast<GLint>(shader_source.size());

			glShaderSource(this->id, 1, &data, &size);
			glCompileShader(this->id);
			check_compile_errors(this->id, type);
		}

		void free() { glDeleteShader(id); };
	private:
		ShaderType type;
		uint32_t id;

		std::expected<bool, std::array<char, error_log_size>> check_compile_errors(uint32_t shader, ShaderType type) const
		{
			int success;
			std::array<char, error_log_size> info_log{};

			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetShaderInfoLog(shader, info_log.size(), nullptr, info_log.data());
				return std::unexpected(info_log);
			}

			return true;
		}
	};

	class ShaderProgram
	{
	public:
		
		ShaderProgram(std::initializer_list<Shader> shaders)
		{
			// shader program
			this->id = glCreateProgram();

			std::for_each(shaders.begin(), shaders.end(), [this](Shader& shader) { this->link_shader(shader); });

			glLinkProgram(this->id);
			
			this->check_compile_errors(this->id);

			// delete the shaders as they're linked into our program now and no longer necessary
			std::for_each(shaders.begin(), shaders.end(), [this](Shader& shader) { shader.free(); });
		}

		void link_shader(Shader shader) { glAttachShader(this->id, shader.id); };
		void use() { glUseProgram(id); };

		void free() { glDeleteProgram(id); };

		// utility uniform functions

		void set_bool(std::string_view name, bool value) const
		{
			set_int(name, static_cast<int>(value));
		}

		void set_int(std::string_view name, int value) const
		{
			glUniform1i(glGetUniformLocation(id, name.data()), value);
		}

		void set_float(std::string_view name, float value) const
		{
			glUniform1f(glGetUniformLocation(id, name.data()), value);
		}

		void set_vec2(std::string_view name, float x, float y) const
		{
			glUniform2f(glGetUniformLocation(id, name.data()), x, y);
		}

		void set_vec3(std::string_view name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(id, name.data()), x, y, z);
		}
		
		void set_vec4(std::string_view name, float x, float y, float z, float w) const
		{
			glUniform4f(glGetUniformLocation(id, name.data()), x, y, z, w);
		}


		void set_mat4(std::string_view name, std::span<float, 16> matrix) const
		{
			glUniformMatrix4fv(glGetUniformLocation(id, name.data()), 1, GL_FALSE, matrix.data());
		}


	private:
		uint32_t id;

		std::expected<bool, std::array<char, error_log_size>> check_compile_errors(uint32_t shader) const
		{
			int success;
			std::array<char, error_log_size> info_log{};

			glGetProgramiv(shader, GL_COMPILE_STATUS, &success);

			if (!success)
			{
				glGetProgramInfoLog(shader, info_log.size(), nullptr, info_log.data());
				return std::unexpected(info_log);
			}

			return true;
		}
	};
}

#endif // !ODISHGL_SHADER_H
