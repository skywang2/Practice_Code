#pragma once
#include "Test.h"
#include <memory>
#include <vector>
#include <iostream>
//#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../include/imgui/imgui.h"

#include "../Renderer.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexArray.h"
#include "../VertexBufferLayout.h"
#include "../Shader.h"
#include "../Texture.h"
#include "../Model.h"

namespace tests {
	//使用assimp读取模型数据
	//使用自定义Mesh类存储、渲染网格对象
	class TestMesh1 : public Test
	{
	public:
		TestMesh1();
		~TestMesh1();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;
		void ProcessInputClass(GLFWwindow*) override;

	private:
		int display_w, display_h;

		glm::vec3 model_trans;//模型变换平移
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;//模型变换
		glm::mat4 mvp;
		float fov, zNear, zFar;
		glm::vec3 cameraPos;//视图变换平移
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> outlingShader;
		std::unique_ptr<Shader> planeShader;
        std::vector<Mesh> m_objects;
		Model m_3DModel;
		Material m_lightMaterial;

        //帧缓冲相关
        unsigned int framebuffer;//帧缓冲对象
        unsigned int texCololrBuffer;//空纹理对象
        unsigned int rbo;//渲染缓冲对象
        unsigned int vao_f;
        unsigned int vbo_f;
        unsigned int ebo_f;
        std::unique_ptr<Shader> framebufferShader;

        //天空盒
        unsigned int skyboxBuffer;//纹理
        unsigned int vao_sky;
        unsigned int vbo_sky;
        std::unique_ptr<Shader> skyboxShader;

        //几何着色器绘制法线
        std::unique_ptr<Shader> normalShader;

    private:
        //加载立方体的顶点和着色器，顶点（pos，coords）
        template<typename T1, int N, typename T2, int M>
        bool LoadObjectMesh(T1 (&vertices)[N], T2 (&indices)[M], const char* filename, const std::string& directory)
        {
            int countPerPoint = 8;//顶点坐标(3)+法线坐标(3)+纹理坐标(2)
            if (N != countPerPoint * M)
            {
                //std::cout << "vertex&index count different" << std::cout;
                return false;
            }
            
            std::vector<MeshVertex> vec_vertices;
            std::vector<unsigned int> vec_indices;
            std::vector<MeshTexture> vec_textures;

            for (unsigned short i = 0; i < M; i++)
            {
                MeshVertex vertex;
                unsigned int index;

                memcpy(&vertex, vertices + countPerPoint * i, countPerPoint * sizeof(float));
                vec_vertices.push_back(std::move(vertex));

                index = indices[i];
                vec_indices.push_back(index);
            }
            MeshTexture tex;
            tex.id = TextureFromFile(filename, directory);
            tex.type = "texture_diffuse";//只使用漫反射贴图
            tex.path = "";
            vec_textures.push_back(std::move(tex));

            m_objects.push_back(std::move(Mesh(vec_vertices, vec_indices, vec_textures)));
            return true;
        }
	};

	//立方体顶点数据
    static float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
}//namespace tests
