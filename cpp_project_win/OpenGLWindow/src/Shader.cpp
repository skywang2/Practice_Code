#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "GL/glew.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
    : m_programID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    CreateShader(source.vertexShader, source.fragmentShader);
    //std::cout << "=====vertex=====" << std::endl;
    //std::cout << source.vertexShader << std::endl;
    //std::cout << "=====fragmen=====" << std::endl;
    //std::cout << source.fragmentShader << std::endl;
}

Shader::~Shader()
{
}

void Shader::Bind() const
{
    GLCall(glUseProgram(m_programID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream fileStream(filepath);
    if (!fileStream.is_open())
    {
        std::cout << "open file:" << filepath << " failed" << std::endl;
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

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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

bool Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    m_programID = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    int result = -1;

    glAttachShader(m_programID, vs);
    glAttachShader(m_programID, fs);

    glLinkProgram(m_programID);//将shader对象链接到program对象上，链接一个GPU（可编程顶点处理器）上的可执行对象
    glGetProgramiv(m_programID, GL_LINK_STATUS, &result);
    if (GL_FALSE == result)
    {
        int length = 0;
        glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//alloca可以在栈上分配内存
        glGetProgramInfoLog(m_programID, length, &length, message);
        std::cout << message << std::endl;
        glDeleteProgram(m_programID);
        return false;
    }
    glValidateProgram(m_programID);//验证程序对象

    glDetachShader(m_programID, vs);
    glDetachShader(m_programID, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return true;
}