#include "TestTexture2D.h"
//#include "GLFW/glfw3.h"
#include "../include/imgui/imgui.h"


namespace tests {
	TestTexture2D::TestTexture2D()
		: positions{
			50.0f, 50.0f, 1.0f, 1.0f,//前两个是2D点坐标，后两个是对应的纹理坐标
			-50.f, -50.f, 0.0f, 0.0f,
			50.0f, -50.f, 1.0f, 0.0f,
			-50.f, 50.0f, 0.0f, 1.0f}
		, indices	{0, 1, 2, 0, 1, 3}
		, display_w(1280)
		, display_h(720)
		, translation(50, 50, 0)
		, view(glm::mat4(1.0f))
		, vao()
		, vbo(positions, 4 * 4 * sizeof(float))
		, layoutPosition()
		, ibo(indices, 6 * sizeof(unsigned int))
		, shader("res/shaders/allShaders.shader")
		, texture("res/textures/texture01.png")
		, m_renderer()
	{
		layoutPosition.Push<float>(2);//顶点坐标
		layoutPosition.Push<float>(2);//纹理坐标
		vao.AddBuffer(vbo, layoutPosition);

		shader.Bind();
		shader.SetUniform4f("u_color", 0.0f, 0.0f, 0.3f, 1.0f);//此处未使用该值，可忽略

		int texSlot = 0;//纹理槽的下标
		texture.Bind(texSlot);
		shader.SetUniform1i("u_texture", texSlot);

	}

	TestTexture2D::~TestTexture2D()
	{
		vao.Unbind();
		vbo.Unbind();
		ibo.Unbind();
		shader.Unbind();
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
		proj = glm::ortho(0.0f, (float)display_w, 0.0f, (float)display_h, -1.0f, 1.0f);//正交矩阵
		view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(glm::mat4(1.0f), translation);//模型矩阵
		mvp = proj * view * model;
		shader.SetUniformMat4f("u_MVP", mvp);//传入MVP矩阵
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		m_renderer.Draw(vao, ibo, shader);//绘制命令
	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat("test2_x", &translation.x, 0.0f, (float)display_w);
		ImGui::SliderFloat("test2_y", &translation.y, 0.0f, (float)display_h);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//显示帧率
	}

}