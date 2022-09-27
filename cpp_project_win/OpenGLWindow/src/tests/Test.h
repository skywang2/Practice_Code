#pragma once
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <iostream>

struct GLFWwindow;

namespace tests {

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate() {}//����״̬������
		virtual void OnRender() {}//Ҫ��Ⱦ��ͼ��
		virtual void OnImGuiRender() {}//Ҫ��Ⱦ��imgui�ؼ�
		virtual void ProcessInputClass(GLFWwindow*) {}//�����¼�
	};

	class TestMenu : public Test
	{
	public:
		TestMenu();
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string name)
		{
			std::cout << "register " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}

		Test* GetCurrentTest() { return m_currentTest; }
		void SetCurrentTest(Test* p) { m_currentTest = p; }

	private:
		Test* m_currentTest;
		std::vector<std::pair<std::string, std::function<Test * ()>>> m_Tests;//����Ӧ�����ʹ�������
	};
}
