#pragma once
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;
	unsigned int normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:					return sizeof(GLfloat);
		case GL_UNSIGNED_INT:		return sizeof(GLfloat);
		case GL_UNSIGNED_BYTE:	return sizeof(GLfloat);
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	typedef VertexBufferElement Element;
	std::vector<Element> m_element;
	unsigned int m_stride;

public:
	VertexBufferLayout();
	~VertexBufferLayout();

	//ʹ��ģ�庯����c++��֧��ģ�����ʽ���룬��ģ��Ҫ����ͷ�ļ���
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);//��̬���ԣ������ڼ��
	}

	//ƫ�ػ�һ���������ñ����������������õĺ���
	template<>
	void Push<float>(unsigned int count)
	{
		m_element.push_back({ count, GL_FLOAT, GL_FALSE });
		//m_stride += count * sizeof(GLfloat);
		m_stride += count * Element::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_element.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		//m_stride += sizeof(GLuint);
		m_stride += count * Element::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_element.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
		//m_stride += sizeof(GLchar);
		m_stride += count * Element::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<Element> GetElement() const { return m_element; }
	inline unsigned int GetStride() const { return m_stride; }
};
