#include "IndexBuffer.h"
#include "Renderer.h"
#include "GL/glew.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_id(0),
	m_count(count)
{
	GLCall(glGenBuffers(1, &m_id));//申请一块buffer并得到他的地址
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));//GL状态机会自动关联GL_ARRAY_BUFFER与GL_ELEMENT_ARRAY_BUFFER对应的顶点
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_id));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
}

void IndexBuffer::Unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

