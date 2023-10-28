#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include "Texture2D.h"
#include "GLSLShader.h"
#include "OdisMath.h"

class SpriteRenderer
{
public:
	SpriteRenderer(GLSLShader& shader);
	~SpriteRenderer();

	void draw_texture(Texture2D& texture, vec2 position, float rotation);

	void draw();
private:
	GLSLShader shader;
	unsigned int quad_vao;
};

#endif