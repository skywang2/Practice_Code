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

	//与TestCube类的区别是模型顶点坐标在[-1,1]
	//对应顶点、索引、shader文件是cube02
	//尝试生成一个带自制纹理的立方体，并且使用键盘wasd控制摄像机前后左右平移，以及鼠标平移控制偏航角yaw和俯仰角pitch
	class TestCubeLight : public Test
	{
	public:
		TestCubeLight();
		~TestCubeLight();

		void OnUpdate() override;
		void OnRender() override;
		void OnImGuiRender() override;
		void ProcessInputClass(GLFWwindow*) override;

	private:
		int display_w, display_h;
		double deltaTime, lastFrame;//当前帧与上一帧所消耗时间差；上一帧消耗时间

		glm::vec3 model_trans;//模型变换平移
		glm::mat4 proj;
		glm::mat4 view;
		glm::mat4 model;//模型变换
		glm::mat4 mvp;
		float fov, zNear, zFar;
		glm::vec3 cameraPos;//视图变换平移
		glm::vec3 cameraFront;//镜头方向
		glm::vec3 cameraUp;//相机头顶方向
		
		std::unique_ptr<VertexArray> vao;//vertex buffer内存布局
		std::unique_ptr<VertexArray> vaoLight;//光源
		std::unique_ptr<VertexBuffer> vbo;
		std::unique_ptr<IndexBuffer> ibo;
		std::unique_ptr<Shader> shader;
		std::unique_ptr<Shader> shaderLight;
		std::unique_ptr<Texture> m_diffuseMap;
		std::unique_ptr<Texture> m_specularMap;
		VertexBufferLayout layoutPosition;
		Renderer m_renderer;

		glm::vec3 m_lightPos;//光源位置，模型坐标
		//glm::vec3 m_lightColor;//光源颜色
		//glm::vec3 m_objectColor;//物体颜色
		Material m_material;
		Material m_lightMaterial;
		ImVec4 m_clear_color;

		bool m_isRoundMove;
	};

}