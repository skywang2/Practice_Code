#include "Model.h"
#include <iostream>

#include "GL/glew.h"
//#include "include/stb_image.h"

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

void Model::Draw(Shader& shader, int count)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader, count);
	}
}

/*
一些比较有用的选项
aiProcess_GenNormals：如果模型不包含法向量的话，就为每个顶点创建法线。
aiProcess_SplitLargeMeshes：将比较大的网格分割成更小的子网格，如果你的渲染有最大顶点数限制，只能渲染较小的网格，那么它会非常有用。
aiProcess_OptimizeMeshes：和上个选项相反，它会将多个小网格拼接为一个大的网格，减少绘制调用从而进行优化。
*/
void Model::loadModel(std::string path)
{
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path
		, aiProcess_Triangulate/*图元全部转为三角形*/ | aiProcess_FlipUVs/*翻转y轴纹理坐标*/);

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
	//处理当前节点所保存的所有mesh索引
	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];//node->mMeshes[i]是索引
		meshes.push_back(processMesh(mesh, scene));
	}

	//递归处理所有子节点
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

	//顶点
	for (int i = 0; i < mesh->mNumVertices; i++)
	{
		MeshVertex vertex;
		glm::vec3 vector;
		//顶点
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;
		//法线
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;
		//纹理坐标
		if (mesh->mTextureCoords[0])//判断是否有纹理坐标
		{
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;//本例中只导入第一组纹理坐标，所以下标填0
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = vec;
			//此处还可读取tangent、bitangent等
		}
		else
		{
			vertex.texcoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}
	//索引
	for (int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}
	//纹理/材质
	//网格只包含了一个指向材质对象的索引
	//一个材质对象(aiMaterial)的内部对每种纹理类型都存储了一个纹理位置数组
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		//漫反射纹理
		std::vector<MeshTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());//在结尾连续插入vector
		//镜面反射纹理
		std::vector<MeshTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());//在结尾连续插入vector
		//法线纹理
		std::vector<MeshTexture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());//在结尾连续插入vector
	}
	return Mesh(vertices, indices, textures);
}

//读取纹理文件并存储到MeshTexture中
std::vector<MeshTexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<MeshTexture> textures;
	for (int i = 0; i < mat->GetTextureCount(type); i++)
	{
		//假设了模型文件中纹理文件的路径是相对于模型文件的本地(Local)路径，比如说与模型文件处于同一目录下
		aiString str;
		mat->GetTexture(type, i, &str);
		MeshTexture tex;
#if 0
		//不使用公共纹理图缓存
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


/*
	认真的读者可能会发现，我们可以基本上忘掉处理任何的节点，
只需要遍历场景对象的所有网格，就不需要为了索引做这一堆复杂的东西了。
我们仍这么做的原因是，使用节点的最初想法是将网格之间定义一个父子关系。
通过这样递归地遍历这层关系，我们就能将某个网格定义为另一个网格的父网格了。
	这个系统的一个使用案例是，当你想位移一个汽车的网格时，
你可以保证它的所有子网格（比如引擎网格、方向盘网格、轮胎网格）都会随着一起位移。
这样的系统能够用父子关系很容易地创建出来。
	然而，现在我们并没有使用这样一种系统，但如果你想对你的网格数据有更多的控制，
通常都是建议使用这一种方法的。这种类节点的关系毕竟是由创建了这个模型的艺术家所定义。
*/

