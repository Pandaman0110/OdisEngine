#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <iostream>

#include <glad/gl.h>

#include "utility/OdisMath.h"
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
				c->log(LogLevel::info, "Framebuffer completed");
			}

			return true;
		}
	public:
		FrameBuffer(int w, int h) : w(w), h(h)
		{
			glGenFramebuffers(1, &fbo);
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);

			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->w, this->h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

			glGenRenderbuffers(1, &rbo);
			glBindRenderbuffer(GL_RENDERBUFFER, rbo);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->w, this->h);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

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
		glm::ivec2 size() const { return glm::ivec2(w, h); };
	};

	typedef FrameBuffer RenderTexture;

}

#endif
