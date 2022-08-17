#pragma once
class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const VertexBuffer& _vb);
	~VertexBuffer();

	VertexBuffer& operator=(const VertexBuffer& _vb);

	void Bind() const;
	void Unbind() const;
private:
	unsigned int m_id;
};

