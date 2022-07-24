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
using std::cout;
using std::endl;

//struct ShaderProgramSource
//{
//    std::string vertexShader;
//    std::string fragmentShader;
//};

//static ShaderProgramSource ParseShader(const std::string& filepath)
//{
//    enum class ShaderType
//    {
//        NONE = -1, VERTEX, FRAGMENT
//    };
//
//    std::ifstream fileStream(filepath);
//    if (!fileStream.is_open())
//    {
//        std::cout << "open file:" << filepath << " failed" << endl;
//        return {};
//    }
//    ShaderType type = ShaderType::NONE;
//    std::string line;
//    std::stringstream shaderStream[2];//vertex;fragment
//    while (std::getline(fileStream, line))
//    {
//        if (line.find("#shader") != std::string::npos)//查找#shader
//        {
//            if (line.find("vertex") != std::string::npos) 
//            {
//                type = ShaderType::VERTEX;
//            }
//            else if (line.find("fragment") != std::string::npos) 
//            {
//                type = ShaderType::FRAGMENT;
//            }
//        }
//        else
//        {
//            shaderStream[static_cast<int>(type)] << line << '\n';
//        }
//    }
//    return { shaderStream[0].str(), shaderStream[1].str() };
//}
//
//static unsigned int CompileShader(unsigned int type, const std::string& source)
//{
//    unsigned int id = glCreateShader(type);//创建shader对象
//    const char* src = source.c_str();
//    glShaderSource(id, 1, &src, nullptr);//绑定shader源码
//    glCompileShader(id);//编译shader源码
//
//    //错误检查，日志
//    int result = -1;  
//    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
//    if (GL_FALSE == result)
//    {
//        int length = 0;
//        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
//        char* message = (char*)alloca(length * sizeof(char));//alloca可以在栈上分配内存
//        glGetShaderInfoLog(id, length, &length, message);
//        std::cout << message << std::endl;
//        glDeleteShader(id);
//        return 0;
//    }
//
//    return id;//返回shader标识符
//}
//
////传入shader源码字符串，构造着色器的公共代码
//static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
//{
//    unsigned int program = glCreateProgram();
//    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
//    int result = -1;
//
//    glAttachShader(program, vs);
//    glAttachShader(program, fs);
//
//    glLinkProgram(program);//将shader对象链接到program对象上，创建一个GPU（可编程顶点处理器）上的可执行对象
//    glGetProgramiv(program, GL_LINK_STATUS, &result);
//    if (GL_FALSE == result)
//    {
//        int length = 0;
//        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
//        char* message = (char*)alloca(length * sizeof(char));//alloca可以在栈上分配内存
//        glGetProgramInfoLog(program, length, &length, message);
//        std::cout << message << std::endl;
//        glDeleteProgram(program);
//        return 0;
//    }
//    glValidateProgram(program);//验证程序对象
//
//    glDetachShader(program, vs);
//    glDetachShader(program, fs);
//
//    glDeleteShader(vs);
//    glDeleteShader(fs);
//
//    return program;
//}

int main(int argc, char* argv[])
{
    if (!glfwInit())
        return -1;
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow*  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
            0.5f, 0.5f,
            -0.5f, -0.5f,
            0.5f, -0.5f,
            -0.5f, 0.5f
        };

        //顶点索引
        unsigned int indices[] =
        {
            0, 1, 2,
            0, 1, 3
        };

        VertexArray vao;
        VertexBuffer vbo(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layoutPosition;
        layoutPosition.Push<float>(2);
        vao.AddBuffer(vbo, layoutPosition);

        IndexBuffer ibo(indices, 6 * sizeof(unsigned int));

        //顶点着色器vertex shader，主要是告诉OpenGL这个顶点在屏幕空间的位置
        //片段着色器/像素着色器，fragment shader/pixels shader
        Shader shader("res/shaders/allShaders.shader");
        //ShaderProgramSource source = ParseShader("res/shaders/allShaders.shader");
        //cout << "=====vertex=====" << endl;
        //cout << source.vertexShader << endl;
        //cout << "=====fragmen=====" << endl;
        //cout << source.fragmentShader << endl;
        //unsigned int shader = CreateShader(source.vertexShader, source.fragmentShader);
        //GLCall(glUseProgram(shader));
        shader.Bind();


        //获取program中的统一变量（全局变量）uniform的地址
        shader.SetUniform4f("u_color", 0.0f, 0.0f, 0.3f, 1.0f);
        //int color = glGetUniformLocation(shader, "u_color");
        //ASSERT(color != -1);//当shader里未使用该变量时，该变量会被优化掉，因此glGetUniformLocation返回-1，或者其他错误情况也会返回-1
        //glUniform4f(color, 0.0f, 0.0f, 0.3f, 1.0f);

        //当顶点数组VAO、顶端缓存VBO、索引缓存IBO等都设置好后进行解绑，再while中逐帧绑定
        vbo.Unbind();
        ibo.Unbind();
        shader.Unbind();
        vao.Unbind();

        float r = 0.0f, g = 0.1f, b = 0.1f;
        float span = 0.01f;
        while (!glfwWindowShouldClose(window))
        {
            ProcessInput(window);//增加额外的按键（事件）处理，设置状态

            GLCall(glClear(GL_COLOR_BUFFER_BIT));
            shader.Bind();
            //GLCall(glUseProgram(shader));//重新绑定shader
            //glUniform4f(color, r, 0.0f, 0.0f, 1.0f);//给uniform赋值，每渲染一帧就要给uniform赋值
            shader.SetUniform4f("u_color", r, 0.0f, 0.0f, 1.0f);
            if (r < 0.0 || r > 1.0) { span *= -1; }
            r += span;
            //重新绑定VAO和IBO，因为VAO包含VBO，并且通过IBO给出索引
            vao.Bind();
            ibo.Bind();
            //绘制命令
            //glDrawArrays(GL_TRIANGLES, 0, 2 * 3);//无索引缓冲区
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//使用顶点索引绘制

            glfwSwapBuffers(window);//双缓冲绘图，交换前后缓冲区
            glfwPollEvents();//检查触发事件，并调用对应的回调函数
        }
    }

    glfwTerminate();//删除释放glfw所有资源
    return 0;
}

//立即模式渲染，已淘汰
/* glBegin(GL_TRIANGLES);
glVertex2d(-0.5f, -0.5f);
glVertex2d(0.f, 0.5f);
glVertex2d(0.5f, -0.5f);
glEnd();*/

