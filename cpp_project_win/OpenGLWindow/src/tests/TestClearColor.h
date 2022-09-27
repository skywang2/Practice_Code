#pragma once
#include "Test.h"

namespace tests {

	class TestClearColor : public Test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		float m_color[4];
	};

}