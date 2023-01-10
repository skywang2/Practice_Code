#pragma once
#include "Test.h"
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../include/imgui/imgui.h"

#include "../Shader.h"
#include "../Model.h"

namespace tests {
	//��������Сʯ��
	//ʹ��Mesh��洢ģ�Ͷ������Ϣ
	class TestPlanet : public Test
	{
	public:
		TestPlanet();
		~TestPlanet();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;
		void ProcessInputClass(GLFWwindow*);//����ʹ��camera�ദ�������̶�����Ĳ���

	private:
		int display_w, display_h;//����ֱ���
		
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;//ģ�ͱ任
		float fov, zNear, zFar;
		glm::vec3 cameraPos;//��ͼ�任ƽ��
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;

		Model m_modelPlanet;
		std::unique_ptr<Shader> m_shaderPlanet;

		Model m_modelRock;
		std::unique_ptr<Shader> m_shaderRock;
		std::vector<glm::mat4> m_modelMatricesRock;

	private:
		/*brief ����ƫ�������飨ģ�;��󣩣���״����ÿ����ʯ��λ��
		[in] count������ƫ��������
		[in] radius���뾶
		[in] offset��Χ�ư뾶��ƫ�ƣ�ʹ��״����һ������
		[out] modelMatrices��ƫ��������*/
		void GenVertexPosition(unsigned int count, float radius, float offset, std::vector<glm::mat4>& modelMatrices);
	};
}