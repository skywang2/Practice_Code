#include "TestPlanet.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../Renderer.h"

extern GLFWwindow* g_window;
extern MouseParam* g_mouseParam;

namespace tests {

TestPlanet::TestPlanet()
	: display_w(1280)
	, display_h(720)
	, proj(glm::mat4(1.0f))
	, view(glm::mat4(1.0f))
	, model(glm::mat4(1.0f))
	, fov(45.f)
	, zNear(1.f), zFar(100.f)
	, cameraPos(glm::vec3(0.0f, 0.0f, 5.0f))
	, cameraFront(glm::vec3(0.0f, 0.0f, -0.1f))//ʹ��g_mouseParam.front���
	, cameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, m_modelPlanet("res/model/planet/planet.obj")
	, m_modelRock("res/model/rock/rock.obj")
{
	//GLCall(glDisable(GL_BLEND));//ʹ�ø߹�����͸��
	GLCall(glEnable(GL_DEPTH_TEST));//������Ȳ���
	//GLCall(glEnable(GL_STENCIL_TEST));//����ģ�����

	m_shaderPlanet.reset(new Shader("res/shaders/shader_model05_vertex_planet.glsl"
		, "res/shaders/shader_model05_fragment_planet.glsl"));
	m_shaderRock.reset(new Shader("res/shaders/shader_model05_vertex_rock.glsl"
		, "res/shaders/shader_model05_fragment_rock.glsl"));

	GenVertexPosition(100, 50.0, 2.5, m_modelMatricesRock);

}

TestPlanet::~TestPlanet()
{
}

void TestPlanet::OnUpdate()
{
	fov = (g_mouseParam) ? g_mouseParam->fov : fov;
	const glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

	proj = glm::perspective(glm::radians(fov), (float)display_w / (float)display_h, zNear, zFar);//͸��ͶӰ
	view = glm::lookAt(cameraPos, cameraPos + mouseMove, cameraUp);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));//ģ�;���ƽ��

	//����
	m_shaderPlanet->Bind();
	//mvp����
	m_shaderPlanet->SetUniformMat4f("u_model", model);//����������ʹ�����model
	m_shaderPlanet->SetUniformMat4f("u_view", view);
	m_shaderPlanet->SetUniformMat4f("u_projection", proj);
	//���λ������
	m_shaderPlanet->SetUniformVec3f("u_viewPos", cameraPos);

}

void TestPlanet::OnRender()
{
	GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearDepth(1.f));

	m_shaderPlanet->Bind();
	m_modelPlanet.Draw(*m_shaderPlanet);//����

#if 1
	//��ʯ
	//����uniform��ֵ��ʽ����ƫ�ƾ���Ч��
	for (unsigned int i = 0; i < m_modelMatricesRock.size(); i++)
	{
		m_shaderRock->SetUniformMat4f("u_model", model + m_modelMatricesRock[i]);
		m_shaderRock->SetUniformMat4f("u_view", view);
		m_shaderRock->SetUniformMat4f("u_projection", proj);

		m_shaderRock->Bind();
		m_modelRock.Draw(*m_shaderRock, 100);	//��ʯ
	}
	//���ö��㻺�����ֵ�����Դ�����ʯ��Ⱦ
#endif
}

void TestPlanet::OnImGuiRender()
{
}

void TestPlanet::ProcessInputClass(GLFWwindow* window)
{
	glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

	float cameraSpeed = 0.1f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		cameraPos += cameraSpeed * mouseMove;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		cameraPos -= cameraSpeed * mouseMove;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		cameraPos -= glm::cross(mouseMove, cameraUp) * cameraSpeed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		cameraPos += glm::cross(mouseMove, cameraUp) * cameraSpeed;
	}
}

void TestPlanet::GenVertexPosition(unsigned int count, float radius, float offset, std::vector<glm::mat4>& modelMatrices)
{
	//������
	if (radius < 0 || offset < 0)
	{
		std::cout << "check input param" << std::endl;
	}

	srand(glfwGetTime());//��ʼ���������

	for (unsigned int i = 0; i < count; i++)
	{
		glm::mat4 model(1.0f);
		//1. λ�ƣ��ֲ��ڰ뾶Ϊ 'radius' ��Բ���ϣ�ƫ�Ƶķ�Χ�� [-offset, offset]
		float angle = (float)i / (float)count * 360.0f;//���㵱ǰ�������ڽǶ�
		float displacement = 0.f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;//ȡ�ཫrand()����ֵ��Χ��С��0-(int)(2 * offset * 100)
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // �����Ǵ��ĸ߶ȱ�x��z�Ŀ���ҪС
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		//2. ���ţ��� 0.05 �� 0.25f ֮�����ţ�����ʯ��С
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		//3. ��ת������һ�����룩���ѡ�����ת�����������������ת����ÿ����ʯ�����Լ��ķ�����̬
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f)/*��ת��*/);
		
		//4. ���ӵ������������
		modelMatrices.push_back(std::move(model));
	}
}

}//namespace