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
			//mesh.set_vertices(std::move(rect_vertices), 2, 2);
		}

		template <VectorType T>
		void draw_frame_buffer(FrameBuffer& frame_buffer, T position, T scale, float rotation)
		{
			shader.use();

			std::vector<float> rect_vertices
			{
				-1.0f, 1.0f,-1.0f, 1.0f,
				-1.0f,-1.0f,-1.0f,-1.0f,
				 1.0f, 1.0f, 1.0f, 1.0f,
				 1.0f,-1.0f, 1.0f,-1.0f
			};

			for (size_t i = 0; i < rect_vertices.size(); i += 2)
			{
				rect_vertices.at(i) *= scale.x ;
				rect_vertices.at(i + 1) *= scale.y;
			}

			mesh.set_vertices(std::move(rect_vertices), 2, 2);
				

			frame_buffer.bind_texture();

			mesh.draw(DrawMode::triangle_strip, mesh.size());
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