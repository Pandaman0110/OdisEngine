#ifndef FRAMEBUFFER_RENDERER_H
#define FRAMEBUFFER_RENDERER_H

#include "FrameBuffer.h"
#include "GLSLShader.h"
#include "utility/OdisMath.h"
#include "Mesh.h"

namespace OdisEngine
{
	class FrameBufferRenderer
	{
	private:
		GLSLShader shader;
		Mesh mesh{};


	public:
		FrameBufferRenderer() {};

		FrameBufferRenderer(GLSLShader& shader) : shader(shader)
		{
			/*
			std::vector<float> rect_vertices
			{
				0.0f, 1.0f, 0.0f, 1.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				1.0f, 1.0f, 1.0f, 1.0f,
				1.0f, 0.0f, 1.0f, 0.0f
			};
			*/

			//create vertices for a rectangles within -1.0 to 1.0 range
			std::vector<float> rect_vertices
			{
				-1.0f,  1.0f, 0.0f, 1.0f, // top left
				-1.0f, -1.0f, 0.0f, 0.0f, // bottom left
				 1.0f,  1.0f, 1.0f, 1.0f, // top right
				 1.0f, -1.0f, 1.0f, 0.0f  // bottom right
			};

			mesh.set_vertices(std::move(rect_vertices), 2, 2);
		}

		void draw_frame_buffer(FrameBuffer& frame_buffer)
		{
			shader.use();

			frame_buffer.bind_texture();

			mesh.draw(DrawPrimitive::triangle_strip, mesh.size());
		}

		void begin_draw(FrameBuffer& frame_buffer)
		{
			frame_buffer.bind_buffer();
		}

		void end_draw()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

	};

}


#endif