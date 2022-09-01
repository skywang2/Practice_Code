#include "TestCubeCoord1.h"
#include "GLFW/glfw3.h"

#include "../LoadModel.h"

extern GLFWwindow* g_window;

namespace tests {
	TestCubeCoord1::TestCubeCoord1()
		: positions{}
		, indices{}
		, display_w(1280)
		, display_h(720)
		, model_trans(glm::vec3(0.0f))
		, view_trans(glm::vec3(0.f, 0.f, 5.f))
		, vao()
		, vbo()
		, layoutPosition()
		, ibo()
		, shader()
		, texture()
		, fov(45.f)
		, zNear(1.f), zFar(100.f)
	{
		//立方体
		LoadVertexAttri<GLfloat>("res/model/cube02_VertexBuffer.txt", positions, 8 * 3);
		unsigned int idxLineNum = LoadVertexAttri<GLuint>("res/model/cube02_IndexBuffer.txt", indices, 12 * 3);

		vao.reset(new VertexArray);
		vbo.reset(new VertexBuffer(positions, 8 * 3 * sizeof(float)));
		ibo.reset(new IndexBuffer(indices, idxLineNum * 3));
		shader.reset(new Shader("res/shaders/shader_cube02_vertex.glsl", "res/shaders/shader_cube02_fragment.glsl"));
		texture.reset(new Texture("res/textures/TestCubeCoord1.png"));

		layoutPosition.Push<float>(3);//顶点坐标
		layoutPosition.Push<float>(2);//纹理坐标
		vao->AddBuffer(*vbo, layoutPosition);

		int texSlot = 0;//纹理槽的下标
		texture->Bind(texSlot);
		shader->SetUniform1i("u_texture", texSlot);
	}

	TestCubeCoord1::~TestCubeCoord1()
	{
		vao->Unbind();
		vbo->Unbind();
		ibo->Unbind();
		shader->Unbind();
	}

	void TestCubeCoord1::OnUpdate(float deltaTime)
	{
		proj = glm::perspective(glm::radians(fov), (float)display_w / (float)display_h, zNear, zFar);//透视投影
		view = glm::lookAt(
			view_trans, // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		model = glm::translate(glm::mat4(1.0f), model_trans);//模型矩阵
		mvp = proj * view * model;
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);//传入MVP矩阵
		//GL_INVALID_VALUE
	}

	void TestCubeCoord1::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glClearDepth(99999.f);

		m_renderer.Draw(*vao, *ibo, *shader);
	}

	void TestCubeCoord1::OnImGuiRender()
	{
		ImGui::ColorEdit3("clear color", (float*)&m_clear_color);//颜色选择器
		ImGui::SliderFloat("model_trans_x", &model_trans.x, -10.f, 10.f);
		ImGui::SliderFloat("model_trans_y", &model_trans.y, -10.f, 10.f);
		ImGui::SliderFloat("model_trans_z", &model_trans.z, -10.f, 10.f);
		ImGui::SliderFloat("view_trans_x", &view_trans.x, -5.f, 5.f);
		ImGui::SliderFloat("view_trans_y", &view_trans.y, -5.f, 5.f);
		ImGui::SliderFloat("view_trans_z", &view_trans.z, -5.f, 5.f);
		ImGui::SliderFloat("fov", &fov, 0.f, 360.f);
		ImGui::SliderFloat("zNear", &zNear, 0.f, 100.f);
		ImGui::SliderFloat("zFar", &zFar, 0.f, 100.f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//显示帧率
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

}