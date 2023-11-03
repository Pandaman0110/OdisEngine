#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "GLSLShader.h"
#include "OdisMath.h"
#include "Color.h"
#include "Font.h"

namespace OdisEngine
{
	class TextRenderer
	{
	private:
		unsigned int vao, vbo;
		GLSLShader shader;
		Font font;
	public:
		TextRenderer() {};
		TextRenderer(GLSLShader& shader);
		~TextRenderer();

		void load_font(Font& font);

		void draw_text(std::string, glm::vec2 pos = {0.0f, 0.0f}, float scale = 1.0f, Color color = {1.0f, 1.0f, 1.0f});

		void draw();
	};

}

#endif