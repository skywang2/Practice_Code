#include "TestPlanet.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "../Renderer.h"

//#define USE_UNIFORM//使用uniform传递偏移矩阵

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
	m_shaderRock.reset(new Shader("res/shaders/shader_model05_vertex_rock.glsl"
		, "res/shaders/shader_model05_fragment_rock.glsl"));

	int rockCount = 1000;
	GenVertexPosition(rockCount, 50.0, 2.5, m_modelMatricesRock);
	std::cout << m_modelMatricesRock.size();

	//使用
	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * m_modelMatricesRock.size(), &m_modelMatricesRock[0], GL_STATIC_DRAW);

	auto rockMeshes = m_modelRock.GetMeshes();
	for (unsigned int i = 0; i < rockMeshes.size(); i++)
	{
		unsigned int VAO = rockMeshes[i].GetVAO();
		glBindVertexArray(VAO);
		// 顶点属性
		GLsizei vec4Size = sizeof(glm::vec4);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
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

#ifdef USE_UNIFORM
#else
	//用顶点缓冲对象赋值，测试大量岩石渲染
	m_shaderRock->SetUniformMat4f("u_view", view);
	m_shaderRock->SetUniformMat4f("u_projection", proj);
#endif

}

void TestPlanet::OnRender()
{
	GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
	GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	GLCall(glClearDepth(1.f));

	m_shaderPlanet->Bind();
	m_modelPlanet.Draw(*m_shaderPlanet);//行星

	//岩石
#ifdef USE_UNIFORM
	//用uniform赋值方式测试偏移矩阵效果
	for (unsigned int i = 0; i < m_modelMatricesRock.size(); i++)
	{
		m_shaderRock->SetUniformMat4f("u_model", /*model + */m_modelMatricesRock[i]);
		m_shaderRock->SetUniformMat4f("u_view", view);
		m_shaderRock->SetUniformMat4f("u_projection", proj);

		m_shaderRock->Bind();
		m_modelRock.Draw(*m_shaderRock, 100);	//岩石
	}
#else
	//用顶点缓冲对象赋值，测试大量岩石渲染
	m_shaderRock->Bind();
	m_modelRock.Draw(*m_shaderPlanet);
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
	//检查入参
	if (radius < 0 || offset < 0)
	{
		std::cout << "check input param" << std::endl;
	}

	srand(glfwGetTime());//初始化随机种子

	for (unsigned int i = 0; i < count; i++)
	{
		glm::mat4 model(1.0f);

		//1. 位移：分布在半径为 'radius' 的圆形上，偏移的范围是 [-offset, offset]
		float angle = (float)i / (float)count * 360.0f;//计算当前对象所在角度
		float displacement = 0.f;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;//取余将rand()返回值范围缩小到0-(int)(2 * offset * 100)
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // 让行星带的高度比x和z的宽度要小
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		//2. 缩放：在 0.05 和 0.25f 之间缩放，将岩石缩小
		float scale = (rand() % 20) / 100.0f + 0.01;
		model = glm::scale(model, glm::vec3(scale));

		//3. 旋转：绕着一个（半）随机选择的旋转轴向量进行随机的旋转，让每个岩石都有自己的飞行姿态
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f)/*旋转轴*/);
		
		//4. 添加到矩阵的数组中
		modelMatrices.push_back(std::move(model));
	}
}

}//namespace