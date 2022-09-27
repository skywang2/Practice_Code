#include "TestCubeCoord1.h"
#include "GLFW/glfw3.h"

#include "../LoadModel.h"

extern GLFWwindow* g_window;
extern MouseParam* g_mouseParam;
//float lastX = 640, lastY = 360;//�������һ֡λ��

namespace tests {
	TestCubeCoord1::TestCubeCoord1()
		: display_w(1280)
		, display_h(720)
		, model_trans(glm::vec3(0.0f))
		, vao()
		, vbo()
		, layoutPosition()
		, ibo()
		, shader()
		, texture()
		, fov(45.f)
		, zNear(1.f), zFar(100.f)
		, cameraPos(glm::vec3(0.0f, 0.0f, 5.0f))
		, cameraFront(glm::vec3(0.0f, 0.0f, -0.1f))//ʹ��g_mouseParam.front���
		, cameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
		, m_lightColor(glm::vec3(1.0))
		, m_toyColor(glm::vec3(1.0f, 0.5f, 0.31f))
	{
		const int valueCountPerPoint = 3 + 2;//��������3��ֵ����������2��ֵ
		float positions[8 * valueCountPerPoint];
		unsigned int indices[12 * 3];
		//������
		LoadVertexAttri<GLfloat>("res/model/cube02_VertexBuffer.txt", positions, 8 * valueCountPerPoint);
		unsigned int idxLineNum = LoadVertexAttri<GLuint>("res/model/cube02_IndexBuffer.txt", indices, 12 * 3);

		vao.reset(new VertexArray);
		vbo.reset(new VertexBuffer(positions, 8 * valueCountPerPoint * sizeof(float)));
		ibo.reset(new IndexBuffer(indices, idxLineNum * 3));
		shader.reset(new Shader("res/shaders/shader_cube02_vertex.glsl", "res/shaders/shader_cube02_fragment.glsl"));
		texture.reset(new Texture("res/textures/TestCubeCoord1.png"));

		layoutPosition.Push<float>(3);//��������
		layoutPosition.Push<float>(2);//��������
		vao->AddBuffer(*vbo, layoutPosition);
		//glDisableVertexAttribArray(1);

		glm::vec3 result = m_lightColor * m_toyColor;

		//glfwSetInputMode(g_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);//����ʾ��꣬�����ڴ����ڼ���겻���Ƴ�����
	}

	TestCubeCoord1::~TestCubeCoord1()
	{
		vao->Unbind();
		vbo->Unbind();
		ibo->Unbind();
		shader->Unbind();
	}

	void TestCubeCoord1::OnUpdate()
	{
		fov = (g_mouseParam) ? g_mouseParam->fov : fov;
		const glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

		proj = glm::perspective(glm::radians(fov), (float)display_w / (float)display_h, zNear, zFar);//͸��ͶӰ
		view = glm::lookAt(
			cameraPos, //���λ������, in World Space
			//cameraPos + cameraFront, //��ͷ������λ��+�������ʹ����ƶ�ʱ����̶����������ĳһ��
			cameraPos + mouseMove,
			cameraUp//����Ϸ���FPS����Ϸ���Ĭ��vec(0.0, 1.0, 0.0)��
		);
		model = glm::translate(glm::mat4(1.0f), model_trans);//ģ�;���
		mvp = proj * view * model;
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);//����MVP����
		//GL_INVALID_VALUE
	}

	void TestCubeCoord1::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glClearDepth(99999.f);

		int texSlot = 0;//�����ۣ�������Ԫ�����±�
		texture->Bind(texSlot);
		shader->SetUniform1i("u_texture", texSlot);//���ܷ��ڹ��캯�����Ҫ������Ⱦѭ���У���������δ�����õ��¸�ֵ�쳣

		m_renderer.Draw(*vao, *ibo, *shader);
	}

	void TestCubeCoord1::OnImGuiRender()
	{
		ImGui::ColorEdit3("clear color", (float*)&m_clear_color);//��ɫѡ����
		ImGui::SliderFloat("model_trans_x", &model_trans.x, -10.f, 10.f);
		ImGui::SliderFloat("model_trans_y", &model_trans.y, -10.f, 10.f);
		ImGui::SliderFloat("model_trans_z", &model_trans.z, -10.f, 10.f);
		ImGui::SliderFloat("cameraPos_x", &cameraPos.x, -15.f, 15.f);
		ImGui::SliderFloat("cameraPos_y", &cameraPos.y, -15.f, 15.f);
		ImGui::SliderFloat("cameraPos_z", &cameraPos.z, -15.f, 15.f);
		if (g_mouseParam)
		{
			ImGui::SliderFloat("front_x", &g_mouseParam->front.x, -15.f, 15.f);
			ImGui::SliderFloat("front_y", &g_mouseParam->front.y, -15.f, 15.f);
			ImGui::SliderFloat("front_z", &g_mouseParam->front.z, -15.f, 15.f);
		}
		ImGui::SliderFloat("fov", &fov, 0.f, 360.f);
		ImGui::SliderFloat("zNear", &zNear, 0.f, 100.f);
		ImGui::SliderFloat("zFar", &zFar, 0.f, 100.f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//��ʾ֡��
		if (ImGui::Button("GL_NEVER")) { GLCall(glDepthFunc(GL_NEVER)); }ImGui::SameLine();
		if (ImGui::Button("GL_ACCUM_BUFFER_BIT")) { GLCall(glDepthFunc(GL_ACCUM_BUFFER_BIT)); }
		if (ImGui::Button("GL_LESS")) { GLCall(glDepthFunc(GL_LESS)); }ImGui::SameLine();
		if (ImGui::Button("GL_EQUAL")) { GLCall(glDepthFunc(GL_EQUAL)); }
		if (ImGui::Button("GL_LEQUAL")) { GLCall(glDepthFunc(GL_LEQUAL)); }ImGui::SameLine();
		if (ImGui::Button("GL_GREATER")) { GLCall(glDepthFunc(GL_GREATER)); }
		if (ImGui::Button("GL_NOTEQUAL")) { GLCall(glDepthFunc(GL_NOTEQUAL)); }ImGui::SameLine();
		if (ImGui::Button("GL_GEQUAL")) { GLCall(glDepthFunc(GL_GEQUAL)); }
		if (ImGui::Button("GL_ALWAYS")) { GLCall(glDepthFunc(GL_ALWAYS)); }
	}

	void TestCubeCoord1::ProcessInputClass(GLFWwindow* window)
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