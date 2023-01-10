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

		Model m_modelPlanet;
		std::unique_ptr<Shader> m_shaderPlanet;

		Model m_modelRock;
		std::unique_ptr<Shader> m_shaderRock;
		std::vector<glm::mat4> m_modelMatricesRock;

	private:
		/*brief 生成偏移量数组（模型矩阵），环状带中每个岩石的位置
		[in] count，生成偏移量个数
		[in] radius，半径
		[in] offset，围绕半径的偏移，使环状带有一定宽度
		[out] modelMatrices，偏移量数组*/
		void GenVertexPosition(unsigned int count, float radius, float offset, std::vector<glm::mat4>& modelMatrices);
	};
}