#include "VertexBuffer.h"
#include "Renderer.h"
#include "GL/glew.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	: m_id(-1)
{
	GLCall(glGenBuffers(1, &m_id));//申请一块buffer并得到他的地址
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_id));//绑定buffer，可能是指定buffer所存储的数据类型
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));//初始化buffer，单位字节
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
