#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include "GLSLShader.h"
#include "utility/OdisMath.h"
#include "Color.h"
#include "Font.h"

#include <glad/gl.h>

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

        TextRenderer(GLSLShader& shader) : shader(shader)
        {
            glGenVertexArrays(1, &this->vao);
            glGenBuffers(1, &this->vbo);
            glBindVertexArray(this->vbo);
            glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }


        ~TextRenderer()
        {
            glDeleteVertexArrays(1, &this->vao);
        }

        void load_font(Font& font) { this->font = font; };

        template <FloatVectorType T1, ColorTypeRGB T2 = ColorRGB>
		void draw_text(std::string text, T1 pos, T2 color, float scale)
		{
            this->shader.use();
            this->shader.set_vector3f("text_color", glm::vec3{ color.r, color.g, color.b });
            glActiveTexture(GL_TEXTURE0);
            glBindVertexArray(this->vao);

            // iterate through all characters
            std::string::const_iterator c;
            auto& characters = this->font.characters;

            for (c = text.begin(); c != text.end(); ++c)
            {
                Character ch = characters.at(*c);

                float xpos = pos.x + ch.bearing.x * scale;
                float ypos = pos.y + (characters['H'].bearing.y - ch.bearing.y) * scale;

                float w = ch.size.x * scale;
                float h = ch.size.y * scale;
                // update VBO for each character
                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 0.0f },

                    { xpos,     ypos + h,   0.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 0.0f }
                };
                // render glyph texture over quad
                glBindTexture(GL_TEXTURE_2D, ch.texture_id);
                // update content of VBO memory
                glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                // render quad
                glDrawArrays(GL_TRIANGLES, 0, 6);
                // now advance cursors for next glyph
                pos.x += (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
            }
            glBindVertexArray(0);
            glBindTexture(GL_TEXTURE_2D, 0);
		}
	};

}

#endif