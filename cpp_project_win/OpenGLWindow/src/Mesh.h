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
    //��������
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

public:
    Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshTexture> textures);
    void Draw(Shader& shader);//ʹ��ָ����ɫ����������

private:
    unsigned int VAO, VBO, EBO;

private:
    void setupMesh();//��ʼ�����㻺��

};
