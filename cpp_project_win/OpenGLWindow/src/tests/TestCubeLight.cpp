#include "TestCubeLight.h"
#include "GLFW/glfw3.h"

#include "../LoadModel.h"

extern GLFWwindow* g_window;
extern MouseParam* g_mouseParam;
//float lastX = 640, lastY = 360;//鼠标在上一帧位置

namespace tests {
	glm::vec3 g_cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-5.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -5.4f, -3.5f),
		glm::vec3(-5.7f,  3.0f, -7.5f),
		glm::vec3(3.3f, -2.0f, -6.5f),
		glm::vec3(3.5f,  2.0f, -4.5f),
		glm::vec3(3.5f,  -5.2f, -1.5f),
		glm::vec3(-1.3f,  -6.0f, -1.5f)
	};
	int g_cubesCount = sizeof(g_cubePositions) / sizeof(glm::vec3);

	TestCubeLight::TestCubeLight()
		: display_w(1280)
		, display_h(720)
		, deltaTime(0.0)
		, lastFrame(0.0)
		, model_trans(glm::vec3(0.0f))
		, vao()
		, vbo()
		, layoutPosition()
		, ibo()
		, shader()
		, m_diffuseMap()
		, fov(45.f)
		, zNear(1.f), zFar(100.f)
		, cameraPos(glm::vec3(0.0f, 0.0f, 5.0f))
		, cameraFront(glm::vec3(0.0f, 0.0f, -0.1f))//使用g_mouseParam.front替代
		, cameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
		, m_lightPos(glm::vec3(3.0f, 3.0f, -3.0f))
		//, m_lightColor(glm::vec3(1.0))
		//, m_objectColor(glm::vec3(1.0f, 0.5f, 0.31f))
		, m_isRoundMove(false)
	{
		const int positionValueCountPerPoint = 3;//顶点坐标3个float
		const int textureValueCountPerPoint = 2;//纹理坐标2个float
		const int normalValueCountPerPoint = 3;//顶点坐标3个float
		const int valueCountPerPoint = positionValueCountPerPoint + textureValueCountPerPoint + normalValueCountPerPoint;//每组顶点数据总float个数
		const int vertexSetCount = 24;//VertexBuffer行数，同一个顶点可能因为纹理坐标、法线坐标不同而独立生成一组
		const int indexSetCount = 12;//IndexBuffer行数

		const int positionCount = vertexSetCount * valueCountPerPoint;
		const int indicesCount = indexSetCount * positionValueCountPerPoint;
		float positions[positionCount];
		unsigned int indices[indicesCount];

		//立方体
		LoadVertexAttri<GLfloat>("res/model/cube03_VertexBuffer_TestCubeLight.txt", positions, positionCount);
		unsigned int idxLineNum = LoadVertexAttri<GLuint>("res/model/cube03_IndexBuffer_TestCubeLight.txt", indices, indicesCount);

		vao.reset(new VertexArray);//背照射物体
		vbo.reset(new VertexBuffer(positions, positionCount * sizeof(float)));
		ibo.reset(new IndexBuffer(indices, indicesCount));
		//shader.reset(new Shader("res/shaders/shader_cube03_vertex_object.glsl", "res/shaders/shader_cube03_fragment_object.glsl"));//使用自定义物体颜色
		shader.reset(new Shader("res/shaders/shader_cube03_vertex_object.glsl", "res/shaders/shader_cube03_fragment_object_diffuse_map.glsl"));//使用漫反射贴图，diffuse map
		m_diffuseMap.reset(new Texture("res/textures/container2.png"));
		m_specularMap.reset(new Texture("res/textures/container2_specular.png"));

		layoutPosition.Push<float>(positionValueCountPerPoint);
		layoutPosition.Push<float>(textureValueCountPerPoint);
		layoutPosition.Push<float>(normalValueCountPerPoint);
		vao->AddBuffer(*vbo, layoutPosition);
		//glDisableVertexAttribArray(1);
		//glDisableVertexAttribArray(2);

		vaoLight.reset(new VertexArray);//光源
		shaderLight.reset(new Shader("res/shaders/shader_cube03_vertex_object.glsl"/*使用与背照射物体相同的vertex shader*/
			, "res/shaders/shader_cube03_fragment_light.glsl"));
		vaoLight->AddBuffer(*vbo, layoutPosition);

		//材质/纹理
		//物体
		float initAmbient[3] = { 1.0f, 0.5f, 0.31f };
		float initDiffuse[3] = { 1.0f, 0.5f, 0.31f };
		float initSpecular[3] = { 0.5f, 0.5f, 0.5f };
		memset(&m_material, 0, sizeof(m_material));
		memcpy_s(&m_material.ambient, 3 * sizeof(float), initAmbient, 3 * sizeof(float));
		memcpy_s(&m_material.diffuse, 3 * sizeof(float), initDiffuse, 3 * sizeof(float));
		memcpy_s(&m_material.specular, 3 * sizeof(float), initSpecular, 3 * sizeof(float));
		m_material.shininess = 32.0f;
		//光源
		float initLightAmbient[3] = { 0.2f, 0.2f, 0.2f };
		float initLightDiffuse[3] = { 0.5f, 0.5f, 0.5f };
		float initLightSpecular[3] = { 1.0f, 1.0f, 1.0f };
		memset(&m_lightMaterial, 0, sizeof(m_lightMaterial));
		memcpy_s(&m_lightMaterial.ambient, 3 * sizeof(float), initLightAmbient, 3 * sizeof(float));
		memcpy_s(&m_lightMaterial.diffuse, 3 * sizeof(float), initLightDiffuse, 3 * sizeof(float));
		memcpy_s(&m_lightMaterial.specular, 3 * sizeof(float), initLightSpecular, 3 * sizeof(float));
	}

	TestCubeLight::~TestCubeLight()
	{
		vao->Unbind();
		vbo->Unbind();
		ibo->Unbind();
		shader->Unbind();
	}

	void TestCubeLight::OnUpdate()
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		if (g_mouseParam) { g_mouseParam->deltaTime = deltaTime; }
		lastFrame = currentFrame;
		//std::cout << "currentFrame:" << currentFrame << std::endl;
		//std::cout << "deltaTime:" << deltaTime << std::endl;
		//std::cout << "lastFrame:" << lastFrame << std::endl;
	}

	void TestCubeLight::OnRender()
	{
		GLCall(glClearColor(m_clear_color.x, m_clear_color.y, m_clear_color.z, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		glClearDepth(99999.f);

		fov = (g_mouseParam) ? g_mouseParam->fov : fov;
		const glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

		proj = glm::perspective(glm::radians(fov), (float)display_w / (float)display_h, zNear, zFar);//透视投影
		view = glm::lookAt(
			cameraPos, //相机位置坐标, in World Space
			//cameraPos + cameraFront, //镜头朝向，用位置+朝向可以使相机移动时朝向固定方向而不是某一点
			cameraPos + mouseMove,
			cameraUp//相机上方向（FPS相机上方向默认vec(0.0, 1.0, 0.0)）
		);
		//model = glm::translate(glm::mat4(1.0f), model_trans);//模型矩阵，单个立方体使用这个model

		glm::mat4 lightModel;
		if (m_isRoundMove)
		{
			m_lightPos = glm::vec3(3.0f * glm::cos(glfwGetTime()), 3.0f, -3.0f * glm::sin(glfwGetTime()));//光源在xz平面做圆周运动
		}
		lightModel = glm::translate(glm::mat4(1.0f), m_lightPos);//光源model矩阵
		lightModel = glm::scale(lightModel, glm::vec3(0.5f, 0.5f, 0.5f));

		shader->Bind();
		//shader->SetUniformMat4f("u_model", model);//单个立方体使用这个model
		shader->SetUniformMat4f("u_view", view);
		shader->SetUniformMat4f("u_projection", proj);
		shader->SetUniformVec3f("u_objectColor", glm::vec3(1.0f, 0.5f, 0.31f));//立方体颜色，被m_material替代
		shader->SetUniformVec3f("u_lightColor", glm::vec3(1.0f));//光源颜色，被m_lightMaterial替代
		shader->SetUniformVec3f("u_lightPos", m_lightPos);
		shader->SetUniformVec3f("u_viewPos", cameraPos);
		shader->SetUniform3f("u_material.ambient", m_material.ambient[0], m_material.ambient[1], m_material.ambient[2]);
		shader->SetUniform3f("u_material.diffuse", m_material.diffuse[0], m_material.diffuse[1], m_material.diffuse[2]);
		shader->SetUniform3f("u_material.specular", m_material.specular[0], m_material.specular[1], m_material.specular[2]);
		shader->SetUniform1f("u_material.shininess", m_material.shininess);
		shader->SetUniform3f("u_lightMaterial.ambient", m_lightMaterial.ambient[0], m_lightMaterial.ambient[1], m_lightMaterial.ambient[2]);
		shader->SetUniform3f("u_lightMaterial.diffuse", m_lightMaterial.diffuse[0], m_lightMaterial.diffuse[1], m_lightMaterial.diffuse[2]);
		shader->SetUniform3f("u_lightMaterial.specular", m_lightMaterial.specular[0], m_lightMaterial.specular[1], m_lightMaterial.specular[2]);

		int texSlot = 0;//纹理槽（纹理单元）的下标
		m_diffuseMap->Bind(texSlot);
		shader->SetUniform1i("u_material.diffuseMap", texSlot);//不能放在构造函数里，需要放在渲染循环中，避免因尚未被调用导致赋值异常
		int texSlotSpecular = 1;//高光贴图
		m_specularMap->Bind(texSlotSpecular);
		shader->SetUniform1i("u_material.specularMap", texSlotSpecular);//不能放在构造函数里，需要放在渲染循环中，避免因尚未被调用导致赋值异常

		shaderLight->Bind();//光源的shader
		shaderLight->SetUniformMat4f("u_model", lightModel);
		shaderLight->SetUniformMat4f("u_view", view);
		shaderLight->SetUniformMat4f("u_projection", proj);

		m_renderer.Draw(*vaoLight, *ibo, *shaderLight);//绘制一个代表光源的立方体

		shader->Bind();
		for (int i = 0; i < g_cubesCount; i++)
		{
			//model = glm::translate(glm::mat4(1.0f), model_trans);//多个箱子重叠，看起来很炫酷
			model = glm::translate(glm::mat4(1.0f), g_cubePositions[i]);//多个立方体使用这个model
			model = glm::rotate(model, glm::radians(20.f * i), glm::vec3(1.0f, 0.3f, 0.5f));
			shader->SetUniformMat4f("u_model", model);
			m_renderer.Draw(*vao, *ibo, *shader);
		}
	}

	void TestCubeLight::OnImGuiRender()
	{
		ImGui::ColorEdit3("clear color", (float*)&m_clear_color);//颜色选择器
		ImGui::SliderFloat("model_trans_x", &model_trans.x, -10.f, 10.f);
		ImGui::SliderFloat("model_trans_y", &model_trans.y, -10.f, 10.f);
		ImGui::SliderFloat("model_trans_z", &model_trans.z, -10.f, 10.f);
		ImGui::SliderFloat("cameraPos_x", &cameraPos.x, -15.f, 15.f);
		ImGui::SliderFloat("cameraPos_y", &cameraPos.y, -15.f, 15.f);
		ImGui::SliderFloat("cameraPos_z", &cameraPos.z, -15.f, 15.f);
		if (g_mouseParam)
		{
			ImGui::SliderFloat("front_x", &g_mouseParam->front.x, -15.f, 15.f);
			ImGui::SliderFloat("front_y", &g_mouseParam->front.y, -15.f, 15.f);
			ImGui::SliderFloat("front_z", &g_mouseParam->front.z, -15.f, 15.f);
		}
		ImGui::SliderFloat("fov", &fov, 0.f, 360.f);
		ImGui::SliderFloat("zNear", &zNear, 0.f, 100.f);
		ImGui::SliderFloat("zFar", &zFar, 0.f, 100.f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//显示帧率
		if (ImGui::Button("round move")) { m_isRoundMove = !m_isRoundMove; }
		ImGui::ColorEdit3("ambient color", m_material.ambient);
		ImGui::ColorEdit3("diffuse color", m_material.diffuse);
		ImGui::ColorEdit3("specular color", m_material.specular);
		ImGui::SliderFloat("shininess", &m_material.shininess, 0.f, 255.f);
		ImGui::ColorEdit3("light ambient color", m_lightMaterial.ambient);
		ImGui::ColorEdit3("light diffuse color", m_lightMaterial.diffuse);
		ImGui::ColorEdit3("light specular color", m_lightMaterial.specular);

	}

	void TestCubeLight::ProcessInputClass(GLFWwindow* window)
	{
		glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

		double delta = (g_mouseParam) ? g_mouseParam->deltaTime : deltaTime;
		float cameraSpeed = 10.0f * delta; // adjust accordingly
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraPos += cameraSpeed * mouseMove;
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraPos -= cameraSpeed * mouseMove;
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraPos -= glm::cross(mouseMove, cameraUp) * cameraSpeed;
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraPos += glm::cross(mouseMove, cameraUp) * cameraSpeed;
		}
	}

}
