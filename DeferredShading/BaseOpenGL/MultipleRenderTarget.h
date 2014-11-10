#pragma once



class MultipleRenderTarget
{
public:
	enum GBufferTextureTarget
	{
		TEXTURE_POSITION = 0,
		TEXTURE_NORMAL,
		TEXTURE_TEXCOORD,
		TEXTURE_DIFFUSE,
		TEXTURE_COUNT
	} ;

public:
	MultipleRenderTarget(void);
	~MultipleRenderTarget(void);

	inline GLuint GetFrameBufferId() const			{ return m_FrameBufferId; }
	inline GLuint GetTextureTargetId(int id) const	{ return m_TextureTarget[id]; }
	inline GLuint GetDepthBufferId() const			{ return m_DepthBuffer; }

	bool Create(GLuint width, GLuint height);
	void ActivateForWriting()						{ glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FrameBufferId); }
	void ActivateForReading()						{ glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FrameBufferId); }
	void SetReadBuffer(GBufferTextureTarget target)	{ glReadBuffer(GL_COLOR_ATTACHMENT0 + target); }
	void Desactivate();

protected:
	GLuint m_Width;
	GLuint m_Height;

	GLuint m_FrameBufferId;
	GLuint m_TextureTarget[TEXTURE_COUNT];
	GLuint m_DepthBuffer;
};

