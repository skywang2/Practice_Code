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

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void SetRenderer(void* renderer) override;

	private:
		float positions[4*4];
		unsigned int indices[2*3];
		glm::vec3 translation;
		glm::mat4 view;

		VertexArray vao;
		VertexBuffer vbo;
		VertexBufferLayout layoutPosition;
		IndexBuffer ibo;
		Shader shader;
		Texture texture;
		Renderer* m_renderer;
	};

}