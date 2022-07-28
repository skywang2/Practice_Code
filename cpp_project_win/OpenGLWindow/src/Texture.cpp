#include "Texture.h"
#include "GL/glew.h"
#include "include/stb_image.h"

Texture::Texture(const std::string& filepath)
	:m_id(0),
	m_filepath(filepath),
	m_width(0),
	m_height(0),
	m_BPP(0)
{
	


	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);


	//GLCall();
}

Texture::~Texture()
{

}

void Texture::Bind(unsigned int slot = 0) const
{

}

void Texture::Unbind() const
{

}
