#pragma once
#include "Test.h"
#include <memory>
//#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../include/imgui/imgui.h"

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

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		float positions[8 * 3];
		unsigned int indices[12 * 3];
		int display_w, display_h;

		glm::vec3 model_trans;//ģ�ͱ任ƽ��
		glm::vec3 view_trans;//��ͼ�任ƽ��
		glm::vec2 zCoord;
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;//ģ�ͱ任
		glm::mat4 mvp;
		float fov, zNear, zFar;

		std::unique_ptr<VertexArray> vao;//vertex buffer�ڴ沼��
		std::unique_ptr<VertexBuffer> vbo;
		std::unique_ptr<IndexBuffer> ibo;
		std::unique_ptr<Shader> shader;
		//std::unique_ptr<Texture> texture;
		VertexBufferLayout layoutPosition;
		Renderer m_renderer;

		ImVec4 m_clear_color;
	};

}