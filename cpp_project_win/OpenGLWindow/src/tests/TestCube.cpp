#include "TestCube.h"
#include "../include/imgui/imgui.h"

#include "../LoadModel.h"

namespace tests {
	TestCube::TestCube()
		: positions{}
		, indices{}
		, display_w(1280)
		, display_h(720)
		, proj()
		, view()
		, translation()
		, model()
		, mvp()
		, vao()
		, vbo()
		, layoutPosition()
		, ibo()
		, shader()
		, texture()
		, m_renderer()
	{
		//typedef decltype(*positions) Type;
		LoadVertexAttri<float>("res/model/VertexBuffer_cube01.txt", positions, 8 * 3);
		LoadVertexAttri<unsigned int>("res/model/IndexBuffer_cube01.txt", indices, 12 * 3);

		vao.reset(new VertexArray);
		vbo.reset(new VertexBuffer(positions, 8 * 3 * sizeof(float)));
		ibo.reset(new IndexBuffer(indices, 12 * 3));
		//shader.reset(new Shader(""));
		//texture.reset();

		//layoutPosition.Push<float>(2);//顶点坐标
		//layoutPosition.Push<float>(2);//纹理坐标
		//vao.AddBuffer(vbo, layoutPosition);

		//shader.Bind();
		//shader.SetUniform4f("u_color", 0.0f, 0.0f, 0.3f, 1.0f);//此处未使用该值，可忽略

		//int texSlot = 0;//纹理槽的下标
		//texture.Bind(texSlot);
		//shader.SetUniform1i("u_texture", texSlot);

	}

	TestCube::~TestCube()
	{
		//vao.Unbind();
		//vbo.Unbind();
		//ibo.Unbind();
		//shader.Unbind();
	}

	void TestCube::OnUpdate(float deltaTime)
	{
		//proj = glm::ortho(0.0f, (float)display_w, 0.0f, (float)display_h, -1.0f, 1.0f);//正交矩阵
		//view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		//model = glm::translate(glm::mat4(1.0f), translation);//模型矩阵
		//mvp = proj * view * model;
		//shader.SetUniformMat4f("u_MVP", mvp);//传入MVP矩阵
	}

	void TestCube::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		//m_renderer.Draw(vao, ibo, shader);//绘制命令
	}

	void TestCube::OnImGuiRender()
	{
		ImGui::SliderFloat("test2_x", &translation.x, 0.0f, (float)display_w);
		ImGui::SliderFloat("test2_y", &translation.y, 0.0f, (float)display_h);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//显示帧率
	}

	//void TestCube::SetRenderer(void* renderer)
	//{
	//	m_renderer = reinterpret_cast<Renderer*>(renderer);
	//}

}