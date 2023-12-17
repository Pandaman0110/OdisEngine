#ifndef ODISGL_TEXTURE_H
#define ODISGL_TEXTURE_H

#include <cstdint>

#include <glad/gl.h>

namespace OdisGL
{
	enum class TextureWrap
	{
		repeat = GL_REPEAT,
		mirrored_repeat = GL_MIRRORED_REPEAT,
		edge_clamp = GL_CLAMP_TO_EDGE,
		border_clamp = GL_CLAMP_TO_BORDER
	};

	enum class TextureFilter
	{
		nearest = GL_NEAREST,
		linear = GL_LINEAR
	};

	enum class TextureFormat
	{
		rgb = GL_RGB,
		rgba = GL_RGBA
	};

	class Texture
	{
	public:
		Texture(uint8_t* data, uint32_t w, uint32_t h, TextureFilter filter_mode, TextureWrap wrap_mode, TextureFormat format)
			: id(0), width(w), height(h), 
			wrap_s(static_cast<uint32_t>(wrap_mode)), wrap_t(static_cast<uint32_t>(wrap_mode)), 
			filter_min(static_cast<uint32_t>(filter_mode)), filter_max(static_cast<uint32_t>(filter_mode)),
			internal_format(static_cast<uint32_t>(format)), data_format(static_cast<uint32_t>(format))
		{
			//generate an id for the texture and bind it
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);

			//set the texture wrapping and filtering options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);

			//load the image data into the texture
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, data_format, GL_UNSIGNED_BYTE, data);


			//glGenerateMipmap(GL_TEXTURE_2D);

			//unbind the texture
			glBindTexture(GL_TEXTURE_2D, 0);
		};

		//write delegating constructores for the other texture formats
		Texture(uint8_t* data, uint32_t w, uint32_t h, TextureFilter filter_mode, TextureWrap wrap_mode)
			: Texture(data, w, h, filter_mode, wrap_mode, TextureFormat::rgb) {};

		Texture(uint8_t* data, uint32_t w, uint32_t h, TextureFilter filter_mode)
			: Texture(data, w, h, filter_mode, TextureWrap::repeat, TextureFormat::rgb) {};

		Texture(uint8_t* data, uint32_t w, uint32_t h)
			: Texture(data, w, h, TextureFilter::linear, TextureWrap::repeat, TextureFormat::rgb) {};

		~Texture();

		void bind() const
		{
			glBindTexture(GL_TEXTURE_2D, id);
		}
	private:
		uint32_t id;
		uint32_t width, height;

		///format of the texture object
		uint32_t internal_format;

		///format of the loaded image
		uint32_t data_format;

		uint32_t wrap_s, wrap_t;
		uint32_t filter_min, filter_max;
	};
}

#endif