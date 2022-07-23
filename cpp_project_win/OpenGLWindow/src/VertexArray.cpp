#include "VertexArray.h"
#include "GL/glew.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
	: m_id(0)
{
	//���������������VAO
	//VAO�Ƕ����������VBO�Ƕ��㻺�����VAO�пɴ���VBO��������һ��
	GLCall(glGenVertexArrays(1, &m_id));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_id));

}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElement();//const����ֻ�ܵ���const����
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));//���ö����������飬indexΪ���Ե�����
		//i������Ϊi�Ķ������ԣ�count����ǰ�����м��������̶���1��2��3��4����offset����ǰ���Ե��ֽ���
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), reinterpret_cast<const void*>(offset)));//�����󶨵�VAO��VBO��������buffer�е����Բ���
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

//void VertexArray::AddLayout(const BufferLayout&)
//{
//}