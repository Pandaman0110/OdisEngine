#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Texture2D.h"
#include "GLSLShader.h"
#include "OdisMath.h"

class SpriteRenderer
{

private:
	GLSLShader shader;
	unsigned int quad_vao;

public:
	SpriteRenderer() {};
	SpriteRenderer(GLSLShader& shader);
	~SpriteRenderer();

	void draw_texture(Texture2D& texture, vec2 position, float rotation);

	void draw();
};

#endif