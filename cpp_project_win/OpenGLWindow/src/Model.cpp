#include "Model.h"
#include <iostream>

#include "GL/glew.h"
#include "include/stb_image.h"

void Model::Draw(Shader& shader)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}

	//meshes[0].Draw(shader);//glass
	//meshes[1].Draw(shader);//leg
	//meshes[2].Draw(shader);//hand
	//meshes[3].Draw(shader);//glass2
	//meshes[4].Draw(shader);//arm
	//meshes[5].Draw(shader);//helmet
	//meshes[6].Draw(shader);//body
}

/*
һЩ�Ƚ����õ�ѡ��
aiProcess_GenNormals�����ģ�Ͳ������������Ļ�����Ϊÿ�����㴴�����ߡ�
aiProcess_SplitLargeMeshes�����Ƚϴ������ָ�ɸ�С����������������Ⱦ����󶥵������ƣ�ֻ����Ⱦ��С��������ô����ǳ����á�
aiProcess_OptimizeMeshes�����ϸ�ѡ���෴�����Ὣ���С����ƴ��Ϊһ��������񣬼��ٻ��Ƶ��ôӶ������Ż���
*/
void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path
		, aiProcess_Triangulate/*ͼԪȫ��תΪ������*/ | aiProcess_FlipUVs/*��תy����������*/);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "assimp load model error:" << import.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));

	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//������ǰ�ڵ������������mesh����
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];//node->mMeshes[i]������
		meshes.push_back(processMesh(mesh, scene));
	}

	//�ݹ鴦�������ӽڵ�
	for (int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<MeshVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<MeshTexture> textures;

	//����
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex vertex;
		glm::vec3 vector;
		//����
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		//����
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		//��������
		if (mesh->mTextureCoords[0])//�ж��Ƿ�����������
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;//������ֻ�����һ���������꣬�����±���0
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = vec;
			//�˴����ɶ�ȡtangent��bitangent��
		}
		else
		{
			vertex.texcoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	//����
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	//����/����
	//����ֻ������һ��ָ����ʶ��������
	//һ�����ʶ���(aiMaterial)���ڲ���ÿ���������Ͷ��洢��һ������λ������
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//����������
		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());//�ڽ�β��������vector
		//���淴������
		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());//�ڽ�β��������vector
		//��������
		std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());//�ڽ�β��������vector
	}
	return Mesh(vertices, indices, textures);
}

//��ȡ�����ļ����洢��MeshTexture��
std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<MeshTexture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		//������ģ���ļ��������ļ���·���������ģ���ļ��ı���(Local)·��������˵��ģ���ļ�����ͬһĿ¼��
		aiString str;
		mat->GetTexture(type, i, &str);
		MeshTexture tex;
#if 0
		//��ʹ�ù�������ͼ����
		tex.id = TextureFromFile(str.C_Str(), directory);
		tex.type = typeName;
		tex.path = str;
		textures.push_back(tex);
#else
		bool skip = false;
		for (int j = 0; j < textures_loaded.size(); j++)
		{
			if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}		
		if (!skip)
		{
			MeshTexture tex;
			tex.id = TextureFromFile(str.C_Str(), directory);
			tex.type = typeName;
			tex.path = str;
			textures.push_back(tex);
			textures_loaded.push_back(tex);
		}
#endif
	}

	return textures;
}

unsigned int TextureFromFile(const char* path, const std::string& directory, bool gamma)
{
	std::string filename = std::string(path);
	filename = directory + '/' + filename;

	unsigned int textureID;
	glGenTextures(1, &textureID);//����id

	int width, height, nrComponents;
	//stbi_set_flip_vertically_on_load(1);	//���·�תͼ��opengl�����½ǿ�ʼ����
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format = GL_RGB;//�����ļ�����ɫ��ʽ
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;//0x1907
		else if (nrComponents == 4)
			format = GL_RGBA;//0x1908

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);//����mipmap

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

/*
	����Ķ��߿��ܻᷢ�֣����ǿ��Ի��������������κεĽڵ㣬
ֻ��Ҫ��������������������񣬾Ͳ���ҪΪ����������һ�Ѹ��ӵĶ����ˡ�
��������ô����ԭ���ǣ�ʹ�ýڵ������뷨�ǽ�����֮�䶨��һ�����ӹ�ϵ��
ͨ�������ݹ�ر�������ϵ�����Ǿ��ܽ�ĳ��������Ϊ��һ������ĸ������ˡ�
	���ϵͳ��һ��ʹ�ð����ǣ�������λ��һ������������ʱ��
����Ա�֤�������������񣨱����������񡢷�����������̥���񣩶�������һ��λ�ơ�
������ϵͳ�ܹ��ø��ӹ�ϵ�����׵ش���������
	Ȼ�����������ǲ�û��ʹ������һ��ϵͳ������������������������и���Ŀ��ƣ�
ͨ�����ǽ���ʹ����һ�ַ����ġ�������ڵ�Ĺ�ϵ�Ͼ����ɴ��������ģ�͵������������塣
*/
