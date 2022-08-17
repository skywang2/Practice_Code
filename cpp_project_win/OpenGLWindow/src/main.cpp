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

#include "tests/Test.h"
#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"
#include "tests/TestCube.h"

using std::cout;
using std::endl;

//创建菜单，添加测试项（使用过程式）
inline void MyMenu(int& page, tests::Test* test)
{
    //创建菜单
    switch (page)
    {
    case 0:
        for (int i = 0; i < 2; ++i)
        {
            char name[2] = { (char)('a' + i), '\0' };
            if (ImGui::Button(name))
            {
                page = i + 1;
            }
        }
        break;
    case 1:
        if (ImGui::Button("<-")) { page = 0; }
        test->OnImGuiRender();//渲染imgui控件
        break;
    case 2:
        if (ImGui::Button("<-")) { page = 0; }
        test->OnImGuiRender();//渲染imgui控件
        break;
    default:
        break;
    }

}

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
    ImGui::StyleColorsDark();
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
        //暂时注释掉，确保看清渲染对象的完整轮廓
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置颜色混合方式

        //创建渲染器
        Renderer renderer;
        
        //创建菜单，添加测试项
        tests::Test* current = nullptr;
        tests::TestMenu* menu = new tests::TestMenu();//初始为菜单页面
        menu->RegisterTest<tests::TestClearColor>("Clear Color");
        menu->RegisterTest<tests::TestTexture2D>("Texture2D");
        menu->RegisterTest<tests::TestCube>("TestCube");

        int display_w, display_h;
        int page = 0;//0-菜单；1-窗口1；2-窗口2；。。。
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();//检查触发事件，并调用对应的回调函数
            ProcessInput(window);//增加额外的按键（事件）处理，设置状态
            glfwGetFramebufferSize(window, &display_w, &display_h);

            glClearColor(0.f, 0.f, 0.f, 1.f);
            renderer.Clear();

            //渲染imgui相关
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            //创建菜单，添加测试项
            current = menu->GetCurrentTest();
            if (current)
            {
                current->OnUpdate(0.f);//做一些渲染准备工作
                current->OnRender();//渲染图形
                ImGui::Begin("Test");
                do
                {
                    if (current != menu && ImGui::Button("<-"))//测试项中按下返回键
                    {
                        //current = menu;
                        menu->SetCurrentTest(menu);
                        break;//跳过下一句，避免同时渲染测试项和菜单按钮
                    }
                    current->OnImGuiRender();//修改TestMenu内部的render指针
                } while (0);
                ImGui::End();
            }

            //渲染imgui窗口
            ImGui::Render();            
            //如果点坐标用[-1,1]表示则需要用glViewport转换到窗口比例（相对坐标），否则使用绝对坐标（窗口变化，物体大小不变）
            glViewport(0, 0, display_w, display_h);//0.5*width*(x+1.0);0.5*height*(y+1.0)
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());//真正的渲染函数

            glfwSwapBuffers(window);//双缓冲绘图，交换前后缓冲区
        }
        
        if (menu)
        {
            delete menu;
            menu = nullptr;
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

