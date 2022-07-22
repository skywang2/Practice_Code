#include "VertexBuffer.h"
#include "Renderer.h"
#include "GL/glew.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	: m_id(-1)
{
	GLCall(glGenBuffers(1, &m_id));//����һ��buffer���õ����ĵ�ַ
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));//��buffer��������ָ��buffer���洢����������
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));//��ʼ��buffer����λ�ֽ�
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_id));
}

void VertexBuffer::Bind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));
}

void VertexBuffer::Unbind()
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}