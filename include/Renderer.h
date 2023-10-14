#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"
#include "World.h"

namespace OdisEngine
{
	class Renderer
	{
	public:
		virtual void render(World world) = 0;

	private:
	};
}

#endif