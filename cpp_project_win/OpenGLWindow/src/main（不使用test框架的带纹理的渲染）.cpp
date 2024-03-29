#define GLEW_STATIC 
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "include/imgui/imgui.h"
#include "include/imgui/imgui_impl_glfw.h"
#include "include/imgui/imgui_impl_opengl3.h"

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

#include "tests/TestClearColor.h"

using std::cout;
using std::endl;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int argc, char* argv[])
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow*  window = glfwCreateWindow(1280/*640*/, 720/*480*/, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);//创建opengl上下文
    glfwSwapInterval(1);//设置交换双缓冲的帧间隔，1表示渲染1帧交换一次

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();//创建imgui的上下文
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    glewExperimental = true;//在核心模式需要，使用扩展函数
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "Error:" << glewGetErrorString(err) << endl;
        return -1;
    }
    cout << "Status: Using GL " << glGetString(GL_VERSION) << endl;//注意函数名
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;

    {
        //顶点坐标
        //float positions[] = {
        //    150.0f, 150.0f, 1.0f, 1.0f,//前两个是2D点坐标，后两个是对应的纹理坐标
        //    50.0f, 50.0f, 0.0f, 0.0f,
        //    150.0f, 50.0f, 1.0f, 0.0f,
        //    50.0f, 150.0f, 0.0f, 1.0f
        //};
        float positions[] = {
            50.0f, 50.0f, 1.0f, 1.0f,//前两个是2D点坐标，后两个是对应的纹理坐标
            -50.f, -50.f, 0.0f, 0.0f,
            50.0f, -50.f, 1.0f, 0.0f,
            -50.f, 50.0f, 0.0f, 1.0f
        };

        //顶点索引
        unsigned int indices[] =
        {
            0, 1, 2,
            0, 1, 3
        };

        //暂时注释掉，确保看清渲染对象的完整轮廓
        //GLCall(glEnable(GL_BLEND));
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置颜色混合方式

        VertexArray vao;
        VertexBuffer vbo(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layoutPosition;
        layoutPosition.Push<float>(2);//顶点坐标
        layoutPosition.Push<float>(2);//纹理坐标
        vao.AddBuffer(vbo, layoutPosition);

        IndexBuffer ibo(indices, 6 * sizeof(unsigned int));

        //顶点着色器vertex shader，主要是告诉OpenGL这个顶点在屏幕空间的位置
        //片段着色器/像素着色器，fragment shader/pixels shader
        Shader shader("res/shaders/allShaders.shader");
        shader.Bind();
        shader.SetUniform4f("u_color", 0.0f, 0.0f, 0.3f, 1.0f);//获取program中的统一变量（全局变量）uniform的地址，并赋值

        Texture texture("res/textures/texture01.png");
        int texSlot = 0;//纹理槽的下标
        texture.Bind(texSlot);
        shader.SetUniform1i("u_texture", texSlot);

        //当顶点数组VAO、顶端缓存VBO、索引缓存IBO等都设置好后进行解绑，再while中逐帧绑定
        vao.Unbind();
        vbo.Unbind();
        ibo.Unbind();
        shader.Unbind();

        Renderer renderer;

        glm::vec3 translationA(50, 50, 0);
        glm::vec3 translationB(150, 150, 0);
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));//视图矩阵，把相机向右移动100，相当于物体向左移动100

        //test::TestClearColor testClearColor;
        int display_w, display_h;
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();//检查触发事件，并调用对应的回调函数
            ProcessInput(window);//增加额外的按键（事件）处理，设置状态
            glfwGetFramebufferSize(window, &display_w, &display_h);

            renderer.Clear();
            //自定义的渲染内容
            {   //第一个图形
                glm::mat4 proj = glm::ortho(0.0f, (float)display_w, 0.0f, (float)display_h, -1.0f, 1.0f);//正交矩阵
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);//模型矩阵
                glm::mat4 mvp = proj * view * model;
                shader.Bind();//绑定shader
                shader.SetUniformMat4f("u_MVP", mvp);//传入MVP矩阵

                renderer.Draw(vao, ibo, shader);//绘制命令
            }
            {   //第二个图形，使用同一个图形和不同的MVP矩阵
                glm::mat4 proj = glm::ortho(0.0f, (float)display_w, 0.0f, (float)display_h, -1.0f, 1.0f);
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();//绑定shader
                shader.SetUniformMat4f("u_MVP", mvp);//传入MVP矩阵

                renderer.Draw(vao, ibo, shader);//绘制命令
            }

            //渲染imgui相关
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            //创建窗口，增加一些控件
            {
                ImGui::Begin("Control");//创建带标题的主窗口
                ImGui::SliderFloat("1_x", &translationA.x, 0.0f, (float)display_w);//第一个图形
                ImGui::SliderFloat("1_y", &translationA.y, 0.0f, (float)display_h);
                ImGui::SliderFloat("2_x", &translationB.x, 0.0f, (float)display_w);//第二个图形
                ImGui::SliderFloat("2_y", &translationB.y, 0.0f, (float)display_h);
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//显示帧率
                ImGui::End();
            }
            //渲染imgui窗口
            ImGui::Render();
            //0.5*width*(x+1.0);0.5*height*(y+1.0)
            //如果点坐标用[-1,1]表示则需要用glViewport转换到窗口比例（相对坐标），否则使用绝对坐标（窗口变化，物体大小不变）
            glViewport(0, 0, display_w, display_h);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());//真正的渲染函数

            test.OnUpdate(0.f);
            test.OnRender();
            test.OnImGuiRender();

            glfwSwapBuffers(window);//双缓冲绘图，交换前后缓冲区
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();//删除释放glfw所有资源

    return 0;
}

//float positions[] = {
//    0.5f, 0.5f, 1.0f, 1.0f,//前两个是2D点坐标，后两个是对应的纹理坐标
//    -0.5f, -0.5f, 0.0f, 0.0f,
//    0.5f, -0.5f, 1.0f, 0.0f,
//    -0.5f, 0.5f, 0.0f, 1.0f
//};

//重新绑定VAO和IBO，因为VAO包含VBO，并且通过IBO给出索引
//vao.Bind();//已包含在renderer.Draw
//ibo.Bind();

//glDrawArrays(GL_TRIANGLES, 0, 2 * 3);//无索引缓冲区

//立即模式渲染，已淘汰
/* glBegin(GL_TRIANGLES);
glVertex2d(-0.5f, -0.5f);
glVertex2d(0.f, 0.5f);
glVertex2d(0.5f, -0.5f);
glEnd();*/

//创建一个按钮和字符串，并排显示
//static int counter = 0;
//if (ImGui::Button("Button")) { counter++; }
//ImGui::SameLine();//把前后两句的控件放在同一行
//ImGui::Text("counter = %d", counter);

//渲染imgui相关
//ImGui_ImplOpenGL3_NewFrame();
//ImGui_ImplGlfw_NewFrame();
//ImGui::NewFrame();
//创建窗口，增加一些控件
//{
//    ImGui::Begin("Hello, world!");//创建带标题的主窗口
//    ImGui::SliderFloat3("translation", &translation.x, 0.0f, (float)display_w);//拖动条
//    ImGui::SliderFloat3("translation", &translation.y, 0.0f, (float)display_h);//拖动条
//ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
//    ImGui::ColorEdit3("clear color", (float*)&clear_color);//颜色选择器
//    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);//显示帧率
//    ImGui::End();
//}
//渲染imgui窗口
//ImGui::Render();
//glClear(GL_COLOR_BUFFER_BIT);
//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());//真正的渲染函数

//MVP矩阵
//glm::mat4 proj = glm::ortho(0.0f, 1280.0f, 0.0f, 720.0f, -1.0f, 1.0f);//正交矩阵
//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-50, 0, 0));//视图矩阵，把相机向右移动100，相当于物体向左移动100
//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 200, 0));//模型矩阵
//glm::mat4 mvp = proj * view * model;
//glm::mat4 proj = glm::ortho(0.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);//正交矩阵
//glm::mat4 mvp = proj;

//呼吸灯效果
//float r = 0.0f, g = 0.1f, b = 0.1f;
//float span = 0.01f;
//shader.SetUniform4f("u_color", r, 0.0f, 0.0f, 1.0f);//在shader中绘制红色呼吸灯效果，目前不用
//if (r < 0.0 || r > 1.0) { span *= -1; }//改变颜色
//r += span;

