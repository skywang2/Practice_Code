#include "TestMesh1.h"
#include "GLFW/glfw3.h"

#include "../LoadModel.h"

/*两种绘制轮廓中心对齐的方式：
1.给轮廓的vertex shader的世界坐标加上法线坐标，即
	gl_Position = u_projection * u_view * u_model * vec4(position + normal * scale, 1.0);//scale为缩放倍率
该方式不需要重新计算model，但可能需要额外vertex shader以区分gl_Position
2.使用glPolygonMode函数，在绘制轮廓时只绘制线段，并改变线段宽度
该方式不需要重新计算model，也不需要额外vertex shader
3.在绘制轮廓时重新计算其model
该方式计算model会复杂一些，不需要额外vertex shader
*/
#define POLYGON_MODE
#define USE_FRAMEBUFFER

extern GLFWwindow* g_window;
extern MouseParam* g_mouseParam;
//float lastX = 640, lastY = 360;//鼠标在上一帧位置

static float planeVertices[] = {
	// positions                // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f, 0.0f, 1.0f, 0.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f, 0.0f, 1.0f, 0.0f,  2.0f, 2.0f
};
static unsigned int planeIndices[] = {
	0, 1, 2,
	3, 4, 5
};

static float frameBufferVertices[] = {
	// positions                // texture Coords 
	0.9f, -0.9f,		1.0f, 0.0f,
	-0.9f, 0.9f,		0.0f, 1.0f,
	-0.9f, -0.9f,	0.0f, 0.0f,

	0.9f, -0.9f,		1.0f, 0.0f,
	0.9f, 0.9f,		1.0f, 1.0f,
	-0.9f, 0.9f,		0.0f, 1.0f,
};
static unsigned int frameBufferIndices[] = {
	0, 1, 2,
	3, 4, 5
};

//不使用索引绘制天空盒
float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

std::vector<std::string> faces
{
	"right.jpg",
	"left.jpg",
	"top.jpg",
	"bottom.jpg",
	"front.jpg",
	"back.jpg"
};

namespace tests {
	TestMesh1::TestMesh1()
		: display_w(1280)
		, display_h(720)
		, model_trans(glm::vec3(0.0f))
		, fov(45.f)
		, zNear(1.f), zFar(100.f)
		, cameraPos(glm::vec3(0.0f, 0.0f, 5.0f))
		, cameraFront(glm::vec3(0.0f, 0.0f, -0.1f))//使用g_mouseParam.front替代
		, cameraUp(glm::vec3(0.0f, 1.0f, 0.0f))
		, m_3DModel("res/model/nanosuit/nanosuit.obj")
	{
		GLCall(glDisable(GL_BLEND));//使得高光区域不透明
		//GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置颜色混合方式
		glEnable(GL_STENCIL_TEST);//开启模板测试
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//设置多边形填充模式
		//glEnable(GL_CULL_FACE);//面剔除，剔除背面的三角形，其中默认顶点顺序逆时针为正面

		if (!LoadObjectMesh(planeVertices, planeIndices, "metal.png", "res/textures"))
		{
			std::cout << "load object mesh failed" << std::endl;
		}

		shader.reset(new Shader("res/shaders/shader_model04_vertex.glsl", "res/shaders/shader_model04_fragment.glsl"));
		outlingShader.reset(new Shader("res/shaders/shader_model04_vertex.glsl", "res/shaders/shader_model04_fragment_StencilTesting.glsl"));
		planeShader.reset(new Shader("res/shaders/shader_model04_vertex.glsl", "res/shaders/shader_model04_fragment_plane.glsl"));
		skyboxShader.reset(new Shader("res/shaders/shader_model04_vertex_skybox.glsl", "res/shaders/shader_model04_fragment_skybox.glsl"));
		normalShader.reset(new Shader("res/shaders/shader_model04_Geometry_vertex.glsl",
			"res/shaders/shader_model04_Geometry_fragment.glsl",
			"res/shaders/shader_model04_Geometry_Geometry.glsl"));

#ifdef USE_FRAMEBUFFER
		/*
		0.生成缓冲帧对象
		1.生成空纹理，存储颜色
		2.将空纹理作为颜色缓冲附加到帧缓冲对象
		3.生成渲染缓冲对象，存储深度值和模板值
		4.渲染缓冲对象内部格式设置为GL_DEPTH24_STENCIL8
		5.将渲染缓冲对象附加到帧缓冲对象
		6.检查帧缓冲是否完整
		7.帧缓冲解绑
		8.创建四边形覆盖屏幕，将之前生成的纹理作为四边形的纹理
		9.绘制原画面
		10.绘制四边形
		*/
		glfwGetFramebufferSize(g_window, &display_w, &display_h);
		//0.生成缓冲帧对象
		GLCall(glGenFramebuffers(1, &framebuffer));
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer));
		//1.生成空纹理，存储颜色
		GLCall(glGenTextures(1, &texCololrBuffer));
		GLCall(glBindTexture(GL_TEXTURE_2D, texCololrBuffer));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, display_w, display_h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		//2.将空纹理作为颜色缓冲附加到帧缓冲对象
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texCololrBuffer, 0));
		//3.生成渲染缓冲对象，存储深度值和模板值
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//4.渲染缓冲对象内部格式设置为GL_DEPTH24_STENCIL8
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, display_w, display_h);
		//5.将渲染缓冲对象附加到帧缓冲对象
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
		//6.检查帧缓冲是否完整
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
		}
		//7.帧缓冲解绑
		glBindFramebuffer(GL_FRAMEBUFFER, 0);//不解绑的话，渲染时如果没有指定其他帧缓冲，则自动渲染到该帧缓冲
		//8.创建四边形覆盖屏幕，将之前生成的纹理作为四边形的纹理
		glGenVertexArrays(1, &vao_f);
		glGenBuffers(1, &vbo_f);
		glGenBuffers(1, &ebo_f);

		glBindVertexArray(vao_f);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_f);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), frameBufferVertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_f);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), frameBufferIndices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);//顶点
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
		glEnableVertexAttribArray(1);//纹理
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		framebufferShader.reset(new Shader("res/shaders/shader_model04_vertex_framebuffer.glsl", "res/shaders/shader_model04_fragment_framebuffer.glsl"));
		
#endif

		//加载天空盒
		for (auto& filename : faces)
		{
			filename = "res/textures/skybox/" + filename;
		}
		skyboxBuffer = TextureCubemap(faces);
		//天空盒顶点
		glGenVertexArrays(1, &vao_sky);
		glGenBuffers(1, &vbo_sky);

		glBindVertexArray(vao_sky);
		glBindBuffer(GL_ARRAY_BUFFER, vbo_sky);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);//顶点
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

		//光源
		float initLightAmbient[3] = { 0.2f, 0.2f, 0.2f };
		float initLightDiffuse[3] = { 0.9f, 0.9f, 0.9f };
		float initLightSpecular[3] = { 1.0f, 1.0f, 1.0f };
		memset(&m_lightMaterial, 0, sizeof(m_lightMaterial));
		memcpy_s(&m_lightMaterial.ambient, 3 * sizeof(float), initLightAmbient, 3 * sizeof(float));
		memcpy_s(&m_lightMaterial.diffuse, 3 * sizeof(float), initLightDiffuse, 3 * sizeof(float));
		memcpy_s(&m_lightMaterial.specular, 3 * sizeof(float), initLightSpecular, 3 * sizeof(float));
	}

	TestMesh1::~TestMesh1()
	{
	}

	void TestMesh1::OnUpdate()
	{
		fov = (g_mouseParam) ? g_mouseParam->fov : fov;
		const glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

		proj = glm::perspective(glm::radians(fov), (float)display_w / (float)display_h, zNear, zFar);//透视投影
		view = glm::lookAt(cameraPos, cameraPos + mouseMove, cameraUp);
		model = glm::translate(glm::mat4(1.0f), model_trans);//模型矩阵

		/*物体shader*/
		shader->Bind();
		//mvp矩阵
		shader->SetUniformMat4f("u_model", model);//单个立方体使用这个model
		shader->SetUniformMat4f("u_view", view);
		shader->SetUniformMat4f("u_projection", proj);
		//相机位置坐标
		shader->SetUniformVec3f("u_viewPos", cameraPos);
		//平行光
		shader->SetUniformVec3f("u_directLight.direction", glm::vec3(1.0f, 0.f, 0.f));
		shader->SetUniform3f("u_directLight.ambient", m_lightMaterial.ambient[0], m_lightMaterial.ambient[0], m_lightMaterial.ambient[0]);
		shader->SetUniform3f("u_directLight.diffuse", m_lightMaterial.diffuse[0], m_lightMaterial.diffuse[0], m_lightMaterial.diffuse[0]);
		shader->SetUniform3f("u_directLight.specular", m_lightMaterial.specular[0], m_lightMaterial.specular[0], m_lightMaterial.specular[0]);

		/*轮廓shader*/
#ifdef POLYGON_MODE
		glm::mat4 outlingModel = model;
#else
		glm::mat4 outlingModel = glm::scale(glm::mat4(1.0f), glm::vec3(1.2f, 1.2f, 1.2f)) * model;//缩放
#endif // POLYGON_MODE
		outlingShader->Bind();
		outlingShader->SetUniformMat4f("u_model", outlingModel);
		outlingShader->SetUniformMat4f("u_view", view);
		outlingShader->SetUniformMat4f("u_projection", proj);

		/*地面shader*/
		planeShader->Bind();
		planeShader->SetUniformMat4f("u_model", model);
		planeShader->SetUniformMat4f("u_view", view);
		planeShader->SetUniformMat4f("u_projection", proj);

		/*天空盒shader*/
		glm::mat4 model_sky = glm::scale(glm::mat4(1.0f), glm::vec3(40, 40, 40));
		skyboxShader->Bind();
		skyboxShader->SetUniformMat4f("u_model", model_sky);
		skyboxShader->SetUniformMat4f("u_view", view);
		skyboxShader->SetUniformMat4f("u_projection", proj);

		/*用几何着色器绘制法线*/
		normalShader->Bind();
		normalShader->SetUniformMat4f("u_model", model);
		normalShader->SetUniformMat4f("u_view", view);
		normalShader->SetUniformMat4f("u_projection", proj);
	}

	void TestMesh1::OnRender()
	{
#ifdef USE_FRAMEBUFFER
		//使用帧缓冲，第一阶段绘制
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);//渲染到纹理
#endif // USE_FRAMEBUFFER

		GLCall(glClearColor(0.2f, 0.2f, 0.2f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		glClearDepth(99999.f);

		//绘制天空盒
		GLCall(glDepthMask(GL_FALSE));

		skyboxShader->Bind();
		GLCall(glBindVertexArray(vao_sky));
		GLCall(glActiveTexture(GL_TEXTURE0));//shader中只有一个纹理图
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxBuffer));
		//GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));//绘制
		GLCall(glBindVertexArray(0));

		GLCall(glDepthMask(GL_TRUE));

		//绘制地面及方块
		for (auto& obj : m_objects)
		{
			planeShader->Bind();
			obj.Draw(*planeShader);
		}

		//绘制物体
		glStencilFunc(GL_ALWAYS, 1, 0xFF);//设置比较函数
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//设置模板值更新策略，使得物体位置的模板值为1（ref）
		glStencilMask(0xFF);//启用模板缓冲写入
		shader->Bind();
		m_3DModel.Draw(*shader);

		//绘制轮廓
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//模板值不为1的通过测试，轮廓比物体大，绘制扣掉物体的部分
		glStencilMask(0x00);//禁止模板缓冲更新
		glDisable(GL_DEPTH_TEST);//禁用深度测试
#ifdef POLYGON_MODE
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(10);
		outlingShader->Bind();
		m_3DModel.Draw(*outlingShader);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glLineWidth(1);
#else
		outlingShader->Bind();
		m_3DModel.Draw(*outlingShader);
#endif // POLYGON_MODE
		glStencilMask(0xFF);//允许模板缓冲更新
		glEnable(GL_DEPTH_TEST);//启用深度测试

		//绘制法线
		normalShader->Bind();
		m_3DModel.Draw(*normalShader);

#ifdef USE_FRAMEBUFFER
		//使用帧缓冲，第二阶段绘制
		glBindFramebuffer(GL_FRAMEBUFFER, 0);//渲染到默认帧缓冲
		GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
		glClearDepth(99999.f);

		framebufferShader->Bind();
		glBindVertexArray(vao_f);
		glBindTexture(GL_TEXTURE_2D, texCololrBuffer);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#endif // USE_FRAMEBUFFER

	}

	void TestMesh1::OnImGuiRender()
	{
		ImGui::SliderFloat("ambient.red", &m_lightMaterial.ambient[0], 0.f, 1.f);
		ImGui::SliderFloat("diffuse.red", &m_lightMaterial.diffuse[0], 0.f, 1.f);

	}

	void TestMesh1::ProcessInputClass(GLFWwindow* window)
	{
		glm::vec3& mouseMove = (g_mouseParam) ? g_mouseParam->front : cameraFront;

		float cameraSpeed = 0.1f; // adjust accordingly
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
