#define GLEW_STATIC 
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    if (!glfwInit())
        return -1;
    
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
        float positions[] = {
            0.5f, 0.5f, 1.0f, 1.0f,//前两个是2D点坐标，后两个是对应的纹理坐标
            -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 1.0f
        };

        //顶点索引
        unsigned int indices[] =
        {
            0, 1, 2,
            0, 1, 3
        };

        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));//设置颜色混合方式
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE));//设置颜色混合方式
        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ZERO));//设置颜色混合方式

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

        float r = 0.0f, g = 0.1f, b = 0.1f;
        float span = 0.01f;
        while (!glfwWindowShouldClose(window))
        {
            ProcessInput(window);//增加额外的按键（事件）处理，设置状态

            renderer.Clear();

            shader.Bind();//绑定shader
            shader.SetUniform4f("u_color", r, 0.0f, 0.0f, 1.0f);

            //绘制命令
            renderer.Draw(vao, ibo, shader);
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//使用顶点索引绘制

            if (r < 0.0 || r > 1.0) { span *= -1; }//改变颜色
            r += span;

            glfwSwapBuffers(window);//双缓冲绘图，交换前后缓冲区
            glfwPollEvents();//检查触发事件，并调用对应的回调函数
        }
    }

    glfwTerminate();//删除释放glfw所有资源
    return 0;
}

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

