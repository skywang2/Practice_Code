#pragma once
#include "Test.h"
#include <memory>
//#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "../Renderer.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexArray.h"
#include "../VertexBufferLayout.h"
#include "../Shader.h"
#include "../Texture.h"

namespace tests {

	class TestCube : public Test
	{
	public:
		TestCube();
		~TestCube();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float positions[8 * 3];
		unsigned int indices[12 * 3];
		int display_w, display_h;

		glm::mat4 proj;
		glm::mat4 view;
		glm::vec3 translation;//平移缩放
		glm::mat4 model;//平移缩放
		glm::mat4 mvp;

		std::unique_ptr<VertexArray> vao;//vertex buffer内存布局
		std::unique_ptr<VertexBuffer> vbo;
		std::unique_ptr<IndexBuffer> ibo;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Texture> texture;
		VertexBufferLayout layoutPosition;
		Renderer m_renderer;
	};

}