#pragma once
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma = false);

static std::vector<MeshTexture> textures_loaded;

class Model
{
public:
    Model(std::string path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);

private:
    std::vector<Mesh> meshes;//模型数据
    std::string directory;
    //std::vector<MeshTexture> textures_loaded;

    void loadModel(std::string path);//加载整个模型文件
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

