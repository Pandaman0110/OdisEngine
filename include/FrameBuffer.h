#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <iostream>

#include <glad/gl.h>

#include "Log.h"

namespace OdisEngine
{

	class FrameBuffer
	{
	private:
		unsigned int fbo, rbo;
		unsigned int texture;

		int w, h;

		bool complete()
		{
			auto c = logger->get("OdisEngine");
			bool status = true;
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				c->log(LogLevel::warning, "Framebuffer is not complete");
			}
			else
			{
				c->log(LogLevel::info, "Framebuffer succefully completed");
			}

			return true;
		}
	public:
		FrameBuffer(int w, int h) : w(w), h(h)
		{
			glGenFramebuffers(1, &fbo);
			glGenRenderbuffers(1, &rbo);
			glGenTextures(1, &texture);

			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glBindTexture(GL_TEXTURE_2D, texture);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->w, this->h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glBindTexture(GL_TEXTURE_2D, 0);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->w, this->h);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);

			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			complete();
		}

		void erase()
		{
			glDeleteFramebuffers(1, &fbo);
			glDeleteRenderbuffers(1, &rbo);
			glDeleteTextures(1, &texture);
		}

		void bind_buffer() const
		{
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		}

		void bind_texture() const
		{
			glBindTexture(GL_TEXTURE_2D, texture);
		}

		int width() const { return w; };
		int height() const { return h; };
	};


}

#endif
