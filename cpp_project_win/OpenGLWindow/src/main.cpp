#define GLEW_STATIC 
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using std::cout;
using std::endl;

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#else
#define ASSERT(x) (x)
#endif
//GLCall，带错误打印和debug断点的调用宏
//存在缺点无法用于if三元式，也无法处理返回值非bool的
//#x把x转换成字符串，
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
//glDebugMessageCallback//类似glGetError，可提供更多文本信息

struct ShaderProgramSource
{
    std::string vertexShader;
    std::string fragmentShader;
};

//检查是否存在错误，如果有则阻塞
static void GLClearError()
{
    //glGetError调用时会返回flag并将其置为GL_NO_ERROR，通过循环将所有flag置为GL_NO_ERROR
    while (glGetError() != GL_NO_ERROR);
}

//在每个函数后检查flag
static bool GLLogCall(const char* function, const char* file, int line)
{
    //通过循环返回所有flag
    while (GLenum error = glGetError())
    {
        cout << "[GL_error]"
            << "["<< function << "]"
            << "[" << file << ":" << line << "]"
            << "(0x" << std::hex << error << ")" << endl;
        return false;
    }
    return true;
}

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX, FRAGMENT
    };

    std::ifstream fileStream(filepath);
    if (!fileStream.is_open())
    {
        std::cout << "open file:" << filepath << " failed" << endl;
        return {};
    }
    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream shaderStream[2];//vertex;fragment
    while (std::getline(fileStream, line))
    {
        if (line.find("#shader") != std::string::npos)//查找#shader
        {
            if (line.find("vertex") != std::string::npos) 
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) 
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            shaderStream[static_cast<int>(type)] << line << '\n';
        }
    }
    return { shaderStream[0].str(), shaderStream[1].str() };
}

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

    /* Make the window's context current */
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

    //创建顶点数组对象VAO
    //VAO是顶端数组对象，VBO是顶点缓存对象，VAO中可存多个VBO，像数组一样
    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    unsigned int buffer;
    GLCall(glGenBuffers(1, &buffer));//申请一块buffer并得到他的地址
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));//绑定buffer，可能是指定buffer所存储的数据类型
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW));//初始化buffer，单位字节
    
    GLCall(glEnableVertexAttribArray(0));//启用顶点属性数组
    GLCall(glVertexAttribPointer(0/*没有特殊含义，但必须与shader的layout一样*/,
        2, /*一个顶点有几个数据，此处一个顶点有x、y两个数据*/
        GL_FLOAT, /*数据类型*/
        GL_FALSE, /*标准化*/
        2 * sizeof(float), /*数据跨度，一个顶点有几个字节*/
        nullptr));//定义buffer中的属性布局，并关联VAO与VBO
    
    unsigned int ibo;
    GLCall(glGenBuffers(1, &ibo));//申请一块buffer并得到他的地址
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));//GL状态机会自动关联GL_ARRAY_BUFFER与GL_ELEMENT_ARRAY_BUFFER对应的顶点
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

    //顶点着色器vertex shader，主要是告诉OpenGL这个顶点在屏幕空间的位置
    //片段着色器/像素着色器，fragment shader/pixels shader
    ShaderProgramSource source = ParseShader("res/shaders/allShaders.shader");
    cout << "=====vertex=====" << endl;
    cout << source.vertexShader << endl;
    cout << "=====fragmen=====" << endl;
    cout << source.fragmentShader << endl;
    unsigned int shader = CreateShader(source.vertexShader, source.fragmentShader);
    GLCall(glUseProgram(shader));

    //获取program中的统一变量（全局变量）uniform的地址
    int color = glGetUniformLocation(shader, "u_color");
    ASSERT(color != -1);//当shader里未使用该变量时，该变量会被优化掉，因此glGetUniformLocation返回-1，或者其他错误情况也会返回-1
    glUniform4f(color, 0.0, 0.0, 0.3, 1.0);

    //当顶点数组VAO、顶端缓存VBO、索引缓存IBO等都设置好后进行解绑，再while中逐帧绑定
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));

    float r = 0.0, g = 0.1, b = 0.1;
    float span = 0.01;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);//增加额外的按键（事件）处理，设置状态

        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glUseProgram(shader));//重新绑定shader
        glUniform4f(color, r, 0.0, 0.0, 1.0);//给uniform赋值，每渲染一帧就要给uniform赋值
        if (r < 0.0 || r > 1.0) { span *= -1; }
        r += span;
        //重新绑定VAO和IBO，因为VAO包含VBO，并且通过IBO给出索引
        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
        //绘制命令
        //glDrawArrays(GL_TRIANGLES, 0, 2 * 3);//无索引缓冲区
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));//使用顶点索引绘制
        
        glfwSwapBuffers(window);//双缓冲绘图，交换前后缓冲区
        glfwPollEvents();//检查触发事件，并调用对应的回调函数
    }
    glDeleteProgram(shader);

    glfwTerminate();//删除释放glfw所有资源
    return 0;
}

//立即模式渲染，已淘汰
/* glBegin(GL_TRIANGLES);
glVertex2d(-0.5f, -0.5f);
glVertex2d(0.f, 0.5f);
glVertex2d(0.5f, -0.5f);
glEnd();*/

