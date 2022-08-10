#pragma once
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <iostream>

namespace tests {

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}//更新状态、变量
		virtual void OnRender() {}//要渲染的图形
		virtual void OnImGuiRender() {}//要渲染的imgui控件
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
		std::vector<std::pair<std::string, std::function<Test * ()>>> m_Tests;//保存应用名和创建函数
	};
}