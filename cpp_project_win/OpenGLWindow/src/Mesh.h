#pragma once
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "Shader.h"

struct MeshVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

struct MeshTexture
{
	unsigned int id;
	std::string type;
};


class Mesh
{
public:
    //网格数据
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

public:
    Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    void Draw(Shader& shader);//使用指定着色器绘制网格

private:
    unsigned int VAO, VBO, EBO;

private:
    void setupMesh();//初始化顶点缓冲

};

