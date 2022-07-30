#include "Texture.h"
#include <iostream>
#include "GL/glew.h"
#include "include/stb_image.h"

Texture::Texture(const std::string& filepath)
	:m_id(0),
	m_filepath(filepath),
	m_buffer(nullptr),
	m_width(0),
	m_height(0),
	m_BPP(0)
{ 
	//使用stb加载图像
	stbi_set_flip_vertically_on_load(1);	//上下翻转图像，opengl从左下角开始加载
	m_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_BPP, 4);//后两个参数：每像素的bit数，每像素的通道数，举例：RGBA中每个通道8bit，每像素24bit

	GLCall(glGenTextures(1, &m_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_id));

	//设置纹理参数，注意函数名是Tex
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//设置图像到纹理格式
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height
		, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)m_buffer));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_buffer)
	{
		stbi_image_free(m_buffer);//如果需要对原始数据处理，可以不释放
		m_buffer = nullptr;
	}
}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_id));

	if (m_buffer)
	{
		stbi_image_free(m_buffer);
		m_buffer = nullptr;
	}
}

//把材质绑定到指定的槽
void Texture::Bind(unsigned int slot) const
{
	if (slot >= 32)
	{
		std::cout << "texture slot is not in range" << std::endl;
	}
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));//激活需要绑定的槽
	GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}
