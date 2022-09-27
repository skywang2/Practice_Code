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

	//��TestCube���������ģ�Ͷ���������[-1,1]
	//��Ӧ���㡢������shader�ļ���cube02
	//��������һ�������������������壬����ʹ�ü���wasd���������ǰ������ƽ�ƣ��Լ����ƽ�ƿ���ƫ����yaw�͸�����pitch
	class TestCubeCoord1 : public Test
	{
	public:
		TestCubeCoord1();
		~TestCubeCoord1();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;
		void ProcessInputClass(GLFWwindow*) override;
		friend void MouseCallback(GLFWwindow* window, double xpos, double ypos);//����¼���������
		friend void ScrollCallback(GLFWwindow* window);//�����¼���������

	private:
		int display_w, display_h;

		glm::vec3 model_trans;//ģ�ͱ任ƽ��
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;//ģ�ͱ任
		glm::mat4 mvp;
		float fov, zNear, zFar;
		glm::vec3 cameraPos;//��ͼ�任ƽ��
		glm::vec3 cameraFront;
		glm::vec3 cameraUp;
		
		std::unique_ptr<VertexArray> vao;//vertex buffer�ڴ沼��
		std::unique_ptr<VertexBuffer> vbo;
		std::unique_ptr<IndexBuffer> ibo;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		VertexBufferLayout layoutPosition;
		Renderer m_renderer;

		glm::vec3 m_lightColor;
		glm::vec3 m_toyColor;
		ImVec4 m_clear_color;
	};

}