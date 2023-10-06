#ifndef OPENGLRENDERER_H
#define OPENGLRENDERER_H

#include "Window.h"

namespace OdisEngine
{
	class OpenGLRenderer
	{
	public:
		OpenGLRenderer(Window& window);


	private:
		void window_size_callback(int width, int height);


	};

}


#endif
