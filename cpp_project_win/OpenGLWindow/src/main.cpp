#define GLEW_STATIC 
#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using std::cout;
using std::endl;

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);//创建shader对象
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);//绑定shader源码
    glCompileShader(id);//编译shader源码

    //错误检查，日志
    int result = -1;  
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//alloca可以在栈上分配内存
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;//返回shader标识符
}

//传入shader源码字符串，构造着色器的公共代码
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    int result = -1;

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);//将shader对象链接到program对象上，创建一个GPU（可编程顶点处理器）上的可执行对象
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (GL_FALSE == result)
    {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//alloca可以在栈上分配内存
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << message << std::endl;
        glDeleteProgram(program);
        return 0;
    }
    glValidateProgram(program);//验证程序对象

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

//按键状态处理函数
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(int argc, char* argv[])
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow*  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);//创建opengl上下文

    glewExperimental = true;//在核心模式需要，使用扩展函数
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "Error:" << glewGetErrorString(err) << endl;
        return -1;
    }
    cout << "Status: Using GL " << glGetString(GL_VERSION) << endl;//注意函数名
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;

    float positions[6] = {
        -0.5f, -0.5f,
        0.f, 0.5f,
        0.5f, -0.5f
    };

    //创建顶点数组对象VAO
    //GLuint VertexArrayID;
    //glGenVertexArrays(1, &VertexArrayID);
    //glBindVertexArray(VertexArrayID);

    unsigned int buffer;
    glGenBuffers(1, &buffer);//申请一块buffer并得到他的地址
    glBindBuffer(GL_ARRAY_BUFFER, buffer);//绑定buffer，可能是指定buffer所存储的数据类型
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);//初始化buffer，单位字节
    
    glEnableVertexAttribArray(0);//启用顶点属性数组
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //
    glVertexAttribPointer(0/*没有特殊含义，但必须与shader的layout一样*/,
        2, /*一个顶点有几个数据，此处一个顶点有x、y两个数据*/
        GL_FLOAT, /*数据类型*/
        GL_FALSE, /*标准化*/
        2 * sizeof(float), /*数据跨度，一个顶点有几个字节*/
        nullptr);//定义buffer中的属性布局
    
    //顶点着色器vertex shader，主要是告诉OpenGL这个顶点在屏幕空间的位置
    std::string vertexShader;
    std::ifstream vertexFile("vertexShader.glsl");
    if (vertexFile.is_open())
    {
        std::stringstream s;
        s << vertexFile.rdbuf();
        vertexShader = s.str();
        vertexFile.close();
    }
    //片段着色器/像素着色器，fragment shader/pixels shader
    std::string fragmentShader;
    std::ifstream fragmentFile("fragmentShader.glsl");
    if (fragmentFile.is_open())
    {
        std::stringstream s;
        s << fragmentFile.rdbuf();
        fragmentShader = s.str();
        fragmentFile.close();
    }

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);//增加额外的按键（事件）处理，设置状态

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //立即模式渲染，已淘汰
        /* glBegin(GL_TRIANGLES);
        glVertex2d(-0.5f, -0.5f);
        glVertex2d(0.f, 0.5f);
        glVertex2d(0.5f, -0.5f);
        glEnd();*/

        //渲染指令
        glDrawArrays(GL_TRIANGLES, 0, 3);//无索引缓冲区
        //glDrawElements(GL_TRIANGLES, 3, NULL);//

        glfwSwapBuffers(window);//双缓冲绘图，交换前后缓冲区
        glfwPollEvents();//检查触发事件，并调用对应的回调函数
    }
    glDeleteShader(shader);

    glfwTerminate();//删除释放glfw所有资源
    return 0;
}
