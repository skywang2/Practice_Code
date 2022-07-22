#include "IndexBuffer.h"
#include "Renderer.h"
#include "GL/glew.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_id(-1),
	m_count(0)
{
	GLCall(glGenBuffers(1, &m_id));//����һ��buffer���õ����ĵ�ַ
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));//GL״̬�����Զ�����GL_ARRAY_BUFFER��GL_ELEMENT_ARRAY_BUFFER��Ӧ�Ķ���
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