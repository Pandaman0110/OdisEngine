#ifndef SHAPE_RENDERER_H
#define SHAPE_RENDERER_H

#include "Texture2D.h"
#include "GLSLShader.h"
#include "OdisMath.h"
#include "Color.h"
#include "Mesh.h"

namespace OdisEngine
{
	class ShapeRenderer
	{

	private:
		GLSLShader shader;
		unsigned int vao;

	public:
		ShapeRenderer() {};
		ShapeRenderer(GLSLShader& shader);
		~ShapeRenderer();

		void draw_rect(vec2 pos, vec2 size, float rotation, ColorRGB color, float alpha);

		void draw();
	};
}

#endif