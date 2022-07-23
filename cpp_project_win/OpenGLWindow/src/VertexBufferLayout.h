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

	//使用模板函数，c++不支持模板分离式编译，即模板要放在头文件中
	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);//静态断言，编译期检查
	}

	//偏特化一个参数，让编译器决定真正调用的函数
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

