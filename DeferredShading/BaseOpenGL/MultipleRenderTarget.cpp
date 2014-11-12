#include "stdafx.h"
#include "MultipleRenderTarget.h"



MultipleRenderTarget::MultipleRenderTarget(void)
{

}


MultipleRenderTarget::~MultipleRenderTarget(void)
{

}



bool MultipleRenderTarget::Create(GLuint width, GLuint height)
{
	m_Width = width;
	m_Height = height;

	glGenFramebuffers(1, &m_FrameBufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);

	glGenTextures(TEXTURE_COUNT, m_TextureTarget);
 
	for(unsigned int i = 0; i < TEXTURE_COUNT; ++i)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureTarget[i]);
 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_Width, m_Height, 0, GL_RGB, GL_FLOAT, 0);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_TextureTarget[i], 0);
	}

	glGenTextures(1, &m_DepthBuffer);
	glBindTexture(GL_TEXTURE_2D, m_DepthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthBuffer, 0);
 
	GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3};
	glDrawBuffers(TEXTURE_COUNT, drawBuffers);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE);
}

void MultipleRenderTarget::Desactivate()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, m_Width, m_Height);
}

void MultipleRenderTarget::ActivateForShading()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	for(unsigned int i = 0; i < TEXTURE_COUNT; ++i)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureTarget[TEXTURE_POSITION + i]);
	}
}