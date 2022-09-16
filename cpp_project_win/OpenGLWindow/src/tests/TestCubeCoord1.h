#pragma once
#include "Test.h"
#include <memory>
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

namespace tests {

	//与TestCube类的区别是模型顶点坐标在[-1,1]
	//对应顶点、索引、shader文件是cube02
	class TestCubeCoord1 : public Test
	{
	public:
		TestCubeCoord1();
		~TestCubeCoord1();

		void OnUpdate(float deltaTime) override;
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
		
		std::unique_ptr<VertexArray> vao;//vertex buffer内存布局
		std::unique_ptr<VertexBuffer> vbo;
		std::unique_ptr<IndexBuffer> ibo;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		VertexBufferLayout layoutPosition;
		Renderer m_renderer;

		ImVec4 m_clear_color;
	};

}