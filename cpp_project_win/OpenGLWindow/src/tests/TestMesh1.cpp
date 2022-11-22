#include "TestMesh1.h"
#include "GLFW/glfw3.h"

#include "../LoadModel.h"

/*两种绘制轮廓中心对齐的方式：
1.给轮廓的vertex shader的世界坐标加上法线坐标，即
	gl_Position = u_projection * u_view * u_model * vec4(position + normal * scale, 1.0);//scale为缩放倍率
该方式不需要重新计算model，但可能需要额外vertex shader以区分gl_Position
2.使用glPolygonMode函数，在绘制轮廓时只绘制线段，并改变线段宽度
该方式不需要重新计算model，也不需要额外vertex shader
3.在绘制轮廓时重新计算其model
该方式计算model会复杂一些，不需要额外vertex shader
*/
#define POLYGON_MODE

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
		glEnable(GL_STENCIL_TEST);//开启模板测试
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//设置多边形填充模式

		shader.reset(new Shader("res/shaders/shader_model04_vertex.glsl", "res/shaders/shader_model04_fragment.glsl"));
		outlingShader.reset(new Shader("res/shaders/shader_model04_vertex.glsl", "res/shaders/shader_model04_fragment_StencilTesting.glsl"));
		
		//光源
		float initLightAmbient[3] = { 0.2f, 0.2f, 0.2f };
		float initLightDiffuse[3] = { 0.9f, 0.9f, 0.9f };
		float initLightSpecular[3] = { 1.0f, 1.0f, 1.0f };
		memset(&m_lightMaterial, 0, sizeof(m_lightMaterial));
		memcpy_s(&m_lightMaterial.ambient, 3 * sizeof(float), initLightAmbient, 3 * sizeof(float));
		memcpy_s(&m_lightMaterial.diffuse, 3 * sizeof(float), initLightDiffuse, 3 * sizeof(float));
		memcpy_s(&m_lightMaterial.specular, 3 * sizeof(float), initLightSpecular, 3 * sizeof(float));
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

		//物体shader
		shader->Bind();
		//mvp矩阵
		shader->SetUniformMat4f("u_model", model);//单个立方体使用这个model
		shader->SetUniformMat4f("u_view", view);
		shader->SetUniformMat4f("u_projection", proj);
		//相机位置坐标
		shader->SetUniformVec3f("u_viewPos", cameraPos);
		//平行光
		shader->SetUniformVec3f("u_directLight.direction", glm::vec3(1.0f, 0.f, 0.f));
		shader->SetUniform3f("u_directLight.ambient", m_lightMaterial.ambient[0], m_lightMaterial.ambient[0], m_lightMaterial.ambient[0]);
		shader->SetUniform3f("u_directLight.diffuse", m_lightMaterial.diffuse[0], m_lightMaterial.diffuse[0], m_lightMaterial.diffuse[0]);
		shader->SetUniform3f("u_directLight.specular", m_lightMaterial.specular[0], m_lightMaterial.specular[0], m_lightMaterial.specular[0]);

		//轮廓shader
#ifdef POLYGON_MODE
		glm::mat4 outlingModel = model;
#else
		glm::mat4 outlingModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 1.2f, 1.2f)) * model;//缩放
#endif // POLYGON_MODE
		outlingShader->Bind();
		outlingShader->SetUniformMat4f("u_model", outlingModel);
		outlingShader->SetUniformMat4f("u_view", view);
		outlingShader->SetUniformMat4f("u_projection", proj);
	}

	void TestMesh1::OnRender()
	{
		GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		glClearDepth(99999.f);

		//绘制物体
		glStencilFunc(GL_ALWAYS, 1, 0xFF);//设置比较函数
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//设置模板值更新策略，使得物体位置的模板值为1（ref）
		glStencilMask(0xFF);//启用模板缓冲写入
		shader->Bind();
		m_3DModel.Draw(*shader);

		//绘制轮廓
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//模板值不为1的通过测试，轮廓比物体大，绘制扣掉物体的部分
		glStencilMask(0x00);//禁止模板缓冲更新
		glDisable(GL_DEPTH_TEST);//禁用深度测试
#ifdef POLYGON_MODE
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(10);
		outlingShader->Bind();
		m_3DModel.Draw(*outlingShader);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
#else
		outlingShader->Bind();
		m_3DModel.Draw(*outlingShader);
#endif // POLYGON_MODE
		glStencilMask(0xFF);//允许模板缓冲更新
		glEnable(GL_DEPTH_TEST);//启用深度测试
	}

	void TestMesh1::OnImGuiRender()
	{
		ImGui::SliderFloat("ambient.red", &m_lightMaterial.ambient[0], 0.f, 1.f);
		ImGui::SliderFloat("diffuse.red", &m_lightMaterial.diffuse[0], 0.f, 1.f);

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
