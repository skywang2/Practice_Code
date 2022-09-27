#pragma once
#include "Test.h"
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

	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float positions[4*4];
		unsigned int indices[2*3];
		int display_w, display_h;

		glm::vec3 translation;
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;
		glm::mat4 mvp;

		VertexArray vao;
		VertexBuffer vbo;
		VertexBufferLayout layoutPosition;
		IndexBuffer ibo;
		Shader shader;
		Texture texture;
		Renderer m_renderer;
	};

}