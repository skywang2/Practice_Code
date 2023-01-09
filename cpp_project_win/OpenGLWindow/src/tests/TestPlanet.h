#pragma once
#include "Test.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../include/imgui/imgui.h"

#include "../Shader.h"
#include "../Model.h"

namespace tests {
	//批量绘制小石块
	//使用Mesh类存储模型顶点等信息
	class TestPlanet : public Test
	{
	public:
		TestPlanet();
		~TestPlanet();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;
		void ProcessInputClass(GLFWwindow*);//后续使用camera类处理鼠标键盘对相机的操作

	private:
		int display_w, display_h;//画面分辨率
		
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;//模型变换
		float fov, zNear, zFar;
		glm::vec3 cameraPos;//视图变换平移
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		std::unique_ptr<Shader> m_shaderPlanet;
		Model m_modelPlanet;
		Model m_modelRock;
	private:
		void GenVertexPosition(std::vector<glm::mat4> modelMatrices, unsigned int count);//生成环状带中每个岩石的位置
	};
}