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
	//ʹ��stb����ͼ��
	stbi_set_flip_vertically_on_load(1);	//���·�תͼ��opengl�����½ǿ�ʼ����
	m_buffer = stbi_load(filepath.c_str(), &m_width, &m_height, &m_BPP, 4);//������������ÿ���ص�bit����ÿ���ص�ͨ������������RGBA��ÿ��ͨ��8bit��ÿ����24bit

	GLCall(glGenTextures(1, &m_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_id));

	//��������������ע�⺯������Tex
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//����ͼ��������ʽ
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height
		, 0, GL_RGBA, GL_UNSIGNED_BYTE, (const GLvoid*)m_buffer));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_buffer)
	{
		stbi_image_free(m_buffer);//�����Ҫ��ԭʼ���ݴ��������Բ��ͷ�
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

//�Ѳ��ʰ󶨵�ָ���Ĳ�
void Texture::Bind(unsigned int slot) const
{
	if (slot >= 32)
	{
		std::cout << "texture slot is not in range" << std::endl;
	}
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));//������Ҫ�󶨵Ĳ�
	GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
}

void Texture::Unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}