#pragma once
#include "Test.h"
#include <memory>
#include <vector>
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
        std::vector<Mesh> m_objects;
		Model m_3DModel;
		Material m_lightMaterial;

    private:
        //加载立方体的顶点和着色器，顶点（pos，coords）
        template<typename T1, int N, typename T2, int M>
        bool LoadObjectMesh(T1 vertices[N], T2 indices[M])
        {
            if (N != M)
            {
                std::cout << "vertex&index count different" << std::cout;
                return false;
            }
            
            std::vector<MeshVertex> vec_vertices;
            std::vector<unsigned int> vec_indices;
            std::vector<MeshTexture> vec_textures;

            for (int i = 0; i < N; i++)
            {
                MeshVertex vertex;
                unsigned int index;

                memcpy(&vertex, vertices + 8 * N, 8 * sizeof(float));
                index = indices[i];
                vec_vertices.push_back(std::move(vertex));
                vec_indices.push_back(index);
            }


            m_objects.push_back(std::move(Mesh(vec_vertices, vec_indices, vec_textures)));
            return true;
        }
	};

	//立方体顶点数据
    float cubeVertices[] = {
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
    float planeVertices[] = {
        // positions                // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
         5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
        -5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f,  0.0f, 2.0f,

         5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f,  0.0f, 2.0f,
         5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f,  2.0f, 2.0f
    };
    unsigned int planeIndices[] = {
        0, 1, 2,
        3, 4, 5
    };
}//namespace tests
