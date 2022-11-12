#include "TestMesh1.h"
#include "GLFW/glfw3.h"

#include "../LoadModel.h"

extern GLFWwindow* g_window;
extern MouseParam* g_mouseParam;
//float lastX = 640, lastY = 360;//鼠标在上一帧位置

namespace tests {
	TestMesh1::TestMesh1()
		: display_w(1280)
		, display_h(720)
		, model_trans(glm::vec3(0.0f))
		, fov(45.f)
		, zNear(1.f), zFar(100.f)
		, cameraPos(glm::vec3(0.0f, 0.0f, 5.0f))
		, cameraFront(glm::vec3(0.0f, 0.0f, -0.1f))//使用g_mouseParam.front替代
		, cameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
		, m_3DModel("res/model/nanosuit/nanosuit.obj")
	{
		GLCall(glDisable(GL_BLEND));//使得高光区域不透明
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置颜色混合方式

		shader.reset(new Shader("res/shaders/shader_model04_vertex.glsl", "res/shaders/shader_model04_fragment.glsl"));		
	}

	TestMesh1::~TestMesh1()
	{
	}

	void TestMesh1::OnUpdate()
	{
		fov = (g_mouseParam) ? g_mouseParam->fov : fov;
		const glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

		proj = glm::perspective(glm::radians(fov), (float)display_w / (float)display_h, zNear, zFar);//透视投影
		view = glm::lookAt(cameraPos, cameraPos + mouseMove, cameraUp);
		model = glm::translate(glm::mat4(1.0f), model_trans);//模型矩阵

		shader->Bind();
		shader->SetUniformMat4f("u_model", model);//单个立方体使用这个model
		shader->SetUniformMat4f("u_view", view);
		shader->SetUniformMat4f("u_projection", proj);
	}

	void TestMesh1::OnRender()
	{
		//GLCall(glClearColor(0.437f, 0.585f, 0.808f, 1.f));
		//GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClearColor(0.05f, 0.05f, 0.05f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glClearDepth(99999.f);

		shader->Bind();
		m_3DModel.Draw(*shader);
	}

	void TestMesh1::OnImGuiRender()
	{
	}

	void TestMesh1::ProcessInputClass(GLFWwindow* window)
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

}
