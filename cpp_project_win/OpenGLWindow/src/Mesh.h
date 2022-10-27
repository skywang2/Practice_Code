#pragma once
#include <string>
#include <vector>

#include "glm/glm.hpp"

#include "Shader.h"

struct MeshVertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 texcoords;
};

struct MeshTexture
{
	unsigned int id;
	std::string type;
};


class Mesh
{
public:
    /*  Íø¸ñÊý¾Ý  */
    std::vector<MeshVertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<MeshTexture> textures;

public:
    Mesh(std::vector<MeshVertex> vertices, std::vector<unsigned int> indices, std::vector<MeshVertex> textures);
    void Draw(Shader shader);

private:
    unsigned int VAO, VBO, EBO;

private:
    void setupMesh();

};

