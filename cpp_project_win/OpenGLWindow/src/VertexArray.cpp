#include "VertexArray.h"
#include "GL/glew.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
	: m_id(0)
{
	//创建顶点数组对象VAO
	//VAO是顶端数组对象，VBO是顶点缓存对象，VAO中可存多个VBO，像数组一样
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
	const auto& elements = layout.GetElement();//const对象只能调用const函数
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		////关联绑定的VAO与VBO，并定义buffer中的属性布局
		//i：索引为i的顶点属性，与shader中location一致；
		//count：当前属性有几个数（固定填1、2、3、4）；
		//offset：当前属性的偏移地址（字节）
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride()/*一个点所有属性总长度*/, reinterpret_cast<const void*>(offset)/*当前属性偏移地址*/));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
		GLCall(glEnableVertexAttribArray(i));//启用顶点属性数组，index为属性的索引
	}
} 
