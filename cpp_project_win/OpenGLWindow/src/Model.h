#pragma once
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

class Model
{
public:
    Model(char* path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;//模型数据
    std::string directory;

    void loadModel(std::string path);//加载整个模型文件
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

