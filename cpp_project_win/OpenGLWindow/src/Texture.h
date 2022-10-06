#pragma once
#include "Renderer.h"
#include "glm/vec3.hpp"

struct Material
{
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float shininess;
};

class Texture
{
private:
	unsigned int m_id;
	std::string m_filepath;
	unsigned char* m_buffer;
	int m_width, m_height;
	int m_BPP;//bits pre pixel

public:
	Texture(const std::string& filepath);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_width; }
	inline int GetHeight() const { return m_height; }
};

