#include "Test.h"
#include "imgui/imgui.h"

namespace tests {

TestMenu::TestMenu()
	: m_currentTest(this)
{
}

TestMenu::~TestMenu()
{
}

void TestMenu::OnImGuiRender()
{
	for (auto& test : m_Tests)
	{
		if (ImGui::Button(test.first.c_str()))//判断哪个按钮按下
		{
			m_currentTest = test.second();//创建Test指针，此时还未绘制自定义内容
		}
	}
}

}//namespace tests
