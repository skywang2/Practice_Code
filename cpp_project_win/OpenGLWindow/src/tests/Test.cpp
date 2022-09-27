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
		if (ImGui::Button(test.first.c_str()))//�ж��ĸ���ť����
		{
			m_currentTest = test.second();//����Testָ�룬��ʱ��δ�����Զ�������
		}
	}
}

}//namespace tests