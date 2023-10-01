#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"

namespace OdisEngine 
{
	class Renderer
	{
	public:
		Renderer(const Window& window);

		int swapBuffers();
		void clear();

		void termimate();

	private:
		uint16_t clear_view;

	};

	class Viewport
	{
	public:



	private:


	};

}





#endif