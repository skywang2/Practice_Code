#include "TestCube.h"
#include "../include/imgui/imgui.h"

#include "../LoadModel.h"

namespace tests {
	TestCube::TestCube()
		: positions{}
		, indices{}
		, display_w(1280)
		, display_h(720)
		, model_trans(glm::vec3(0.0f))
		, view_trans(glm::vec3(0.0f))
		, zCoord(glm::vec2(-50.0f, 50.0f))
		//, proj()
		//, view()
		//, model()
		//, mvp()
		, vao()
		, vbo()
		, layoutPosition()
		, ibo()
		, shader()
		//, texture()
		//, m_renderer()
	{
		LoadVertexAttri<GLfloat>("res/model/cube01_VertexBuffer.txt", positions, 8 * 3);
		LoadVertexAttri<GLuint>("res/model/cube01_IndexBuffer.txt", indices, 12 * 3);

		vao.reset(new VertexArray);
		vbo.reset(new VertexBuffer(positions, 8 * 3 * sizeof(float)));
		ibo.reset(new IndexBuffer(indices, 12 * 3));
		shader.reset(new Shader("res/shaders/shader_cube01_vertex.glsl", "res/shaders/shader_cube01_fragment.glsl"));
		//texture.reset();

		layoutPosition.Push<float>(3);//顶点坐标
		//layoutPosition.Push<float>(2);//纹理坐标
		vao->AddBuffer(*vbo, layoutPosition);

		//int texSlot = 0;//纹理槽的下标
		//texture.Bind(texSlot);
		//shader.SetUniform1i("u_texture", texSlot);
	}

	TestCube::~TestCube()
	{
		vao->Unbind();
		vbo->Unbind();
		ibo->Unbind();
		shader->Unbind();
	}

	void TestCube::OnUpdate(float deltaTime)
	{
		//proj = glm::ortho(0.0f, (float)display_w, 0.0f, (float)display_h, zCoord.x, zCoord.y);//正交矩阵
		proj = glm::perspective(glm::radians(45.0f), (float)display_w / (float)display_h, zCoord.x, zCoord.y);//透视投影
		//view = glm::translate(glm::mat4(1.0f), view_trans);
		view = glm::lookAt(
			glm::vec3(100.f, 100.f, 100.f), // Camera is at (4,3,3), in World Space
			glm::vec3(0, 0, 0), // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		model = glm::translate(glm::mat4(1.0f), model_trans);//模型矩阵
		mvp = proj * view * model;
		shader->Bind();
		shader->SetUniformMat4f("u_MVP", mvp);//传入MVP矩阵
		//GL_INVALID_VALUE
	}

	void TestCube::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		m_renderer.Draw(*vao, *ibo, *shader);
		//glDrawArrays(GL_TRIANGLES, 0, 12 * 3);
		//GLCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, nullptr));//使用顶点索引绘制
	}

	void TestCube::OnImGuiRender()
	{
		ImGui::SliderFloat("model_trans_x", &model_trans.x, 0.0f, (float)display_w);
		ImGui::SliderFloat("model_trans_y", &model_trans.y, 0.0f, (float)display_h);
		ImGui::SliderFloat("model_trans_z", &model_trans.z, zCoord.x, zCoord.y);
		ImGui::SliderFloat("view_trans_x", &view_trans.x, 0.0f, (float)display_w);
		ImGui::SliderFloat("view_trans_y", &view_trans.y, 0.0f, (float)display_h);
		ImGui::SliderFloat("view_trans_z", &view_trans.z, -1.f, 1.f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//显示帧率
	}

}