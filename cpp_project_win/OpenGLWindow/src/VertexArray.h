#pragma once
class VertexBuffer;
class VertexBufferLayout;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	
	void Bind() const;
	void Unbind() const;
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

private:
	unsigned int m_id;
};

