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
	, cameraFront(glm::vec3(0.0f, 0.0f, -0.1f))//使用g_mouseParam.front替代
	, cameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
	, m_modelPlanet("res/model/planet/planet.obj")
	, m_modelRock("res/model/rock/rock.obj")
{
	//GLCall(glDisable(GL_BLEND));//使得高光区域不透明
	GLCall(glEnable(GL_DEPTH_TEST));//启用深度测试
	//GLCall(glEnable(GL_STENCIL_TEST));//开启模板测试

	m_shaderPlanet.reset(new Shader("res/shaders/shader_model05_vertex_planet.glsl"
		, "res/shaders/shader_model05_fragment_planet.glsl"));


}

TestPlanet::~TestPlanet()
{
}

void TestPlanet::OnUpdate()
{
	fov = (g_mouseParam) ? g_mouseParam->fov : fov;
	const glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

	proj = glm::perspective(glm::radians(fov), (float)display_w / (float)display_h, zNear, zFar);//透视投影
	view = glm::lookAt(cameraPos, cameraPos + mouseMove, cameraUp);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f));//模型矩阵，平移

	//行星
	m_shaderPlanet->Bind();
	//mvp矩阵
	m_shaderPlanet->SetUniformMat4f("u_model", model);//单个立方体使用这个model
	m_shaderPlanet->SetUniformMat4f("u_view", view);
	m_shaderPlanet->SetUniformMat4f("u_projection", proj);
	//相机位置坐标
	m_shaderPlanet->SetUniformVec3f("u_viewPos", cameraPos);

	//岩石
}

void TestPlanet::OnRender()
{
	GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearDepth(1.f));

	m_shaderPlanet->Bind();
	m_modelPlanet.Draw(*m_shaderPlanet);//行星

#if 1
	m_modelRock.Draw(*m_shaderPlanet/*使用行星的shader*/, 100);	//岩石
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

void TestPlanet::GenVertexPosition(std::vector<glm::mat4> modelMatrices, unsigned int count)
{
	srand(glfwGetTime());//初始化随机种子

}

}//namespace