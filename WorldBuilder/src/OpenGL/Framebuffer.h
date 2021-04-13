#pragma once


class Framebuffer{
public:
	Framebuffer( ) = default;
	Framebuffer(uint32_t Width, uint32_t Height);
	Framebuffer(Framebuffer&& Other);
	~Framebuffer( );

	void Bind( );
	void Unbind( );

	void Resize(uint32_t Width, uint32_t Height);

	uint32_t GetColorId( ) { return m_ColorAttachment; }

private:
	void Create( );

private:
	GLuint m_RendererID = 0;
	uint32_t m_ColorAttachment = 0, m_DepthAttachment = 0;

	uint32_t m_Width = 0, m_Height = 0;
};

