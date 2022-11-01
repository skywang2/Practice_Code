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
		Model m_3DModel;
	};

}