#include "TestClearColor.h"
#include "GL/glew.h"
#include "../include/imgui/imgui.h"
#include "../Renderer.h"

namespace tests {
	TestClearColor::TestClearColor()
		:m_color{0.f, 0.f, 0.f, 0.f}
	{

	}

	TestClearColor::~TestClearColor()
	{

	}

	void TestClearColor::OnUpdate(float deltaTime)
	{

	}

	void TestClearColor::OnRender() 
	{
		GLCall(glClearColor(m_color[0], m_color[1], m_color[2], m_color[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit3("clear color", m_color);//ÑÕÉ«Ñ¡ÔñÆ÷
	}

}