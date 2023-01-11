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
    Model(std::string path)
    {
        loadModel(path);
    }
    void Draw(Shader& shader);
    void Draw(Shader& shader, int count);//实例化批量绘制
    void AddMesh(Mesh mesh) { meshes.push_back(mesh); }

    std::vector<Mesh>& GetMeshes() { return meshes; }

private:
    std::vector<Mesh> meshes;//模型数据
    std::string directory;
    std::vector<MeshTexture> textures_loaded;

    void loadModel(std::string path);//加载整个模型文件
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<MeshTexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

};

