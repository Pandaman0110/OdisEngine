#ifndef RENDERER_H
#define RENDERER_H

#include "Window.h"

namespace OdisEngine 
{
	class Renderer
	{
	public:
		Renderer(Window& window);

		int swap_buffers();
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