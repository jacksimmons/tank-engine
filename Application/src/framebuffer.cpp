#include <glad/glad.h>
#include "framebuffer.hpp"
#include "log.hpp"
#include "shader.hpp"
#include "nodes/scene.hpp"


namespace Tank
{
	Framebuffer::Framebuffer(int width, int height)
	{
		m_w = width;
		m_h = height;

		glGenFramebuffers(1, &m_fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

		// Create texture for colour attachment
		// Require reading of individual pixels, so cannot use a renderbuffer.
		glGenTextures(1, &m_texColBuf);
		glBindTexture(GL_TEXTURE_2D, m_texColBuf);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_w, m_h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColBuf, 0);

		// Create renderbuffer for depth/stencil attachment.
		// Renderbuffers are fast to write to and swap, which is all we need a container of
		// depth/stencil attachments to do.
		glGenRenderbuffers(1, &m_rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_w, m_h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		checkStatus();
	}


	Framebuffer::~Framebuffer()
	{
		glDeleteTextures(1, &m_texColBuf);
		glDeleteFramebuffers(1, &m_fbo);
		glDeleteRenderbuffers(1, &m_rbo);
	}


	void Framebuffer::checkStatus() const
	{
		// Check the framebuffer
		GLenum fbStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fbStatus != GL_FRAMEBUFFER_COMPLETE)
		{
			fprintf(stderr, "Failed to initialise framebuffer. Status: %u", fbStatus);
		}
	}


	/// <summary>
	/// Render a quad whose texture is the whole scene.
	/// </summary>
	void Framebuffer::update() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			glStencilMask(0x00); // Don't write to stencil by default
			Scene *activeScene = Tank::Scene::getActiveScene();
			activeScene->update();
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void Framebuffer::rescale(int w, int h)
	{
		// This section is similar to in ctor, except missing glGen... functions.
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glBindTexture(GL_TEXTURE_2D, m_texColBuf);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColBuf, 0);

		glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

		m_w = w;
		m_h = h;
	}
}