#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
    : m_programID(0),
    m_filepath(filepath)
{
    ShaderProgramSource source = ParseShader(filepath);
    m_programID = CreateShader(source.vertexShader, source.fragmentShader);
    //std::cout << "=====vertex=====" << std::endl;
    //std::cout << source.vertexShader << std::endl;
    //std::cout << "=====fragmen=====" << std::endl;
    //std::cout << source.fragmentShader << std::endl;
}

Shader::Shader(const std::string& vertex, const std::string& fragment)
{
    std::vector<std::string> paths;
    paths.push_back(vertex);
    paths.push_back(fragment);

    std::stringstream ss[2];
    for (int i = 0; i < paths.size(); i++)
    {
        std::fstream file(paths[i]);
        std::string line;
        while (std::getline(file, line))
        {
            ss[i] << line << '\n';
        }        
    }
    m_programID = CreateShader(ss[0].str(), ss[1].str());
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

void Shader::SetUniform1i(const std::string& varible, int v1)
{
    GLint var = GetUniformLocation(varible);
    //glUniform1i(var, v1);
    GLCall(glUniform1i(var, v1));
    //GL_INVALID_OPERATION;
}

void Shader::SetUniform1f(const std::string& varible, float v1)
{
    GLCall(glUniform1f(GetUniformLocation(varible), v1));
}

void Shader::SetUniform3f(const std::string& varible, float v1, float v2, float v3)
{
    GLCall(glUniform3f(GetUniformLocation(varible), v1, v2, v3));
}

void Shader::SetUniform4f(const std::string& varible, float v1, float v2, float v3, float v4)
{
    GLCall(glUniform4f(GetUniformLocation(varible), v1, v2, v3, v4));
}

void Shader::SetUniformVec3f(const std::string& varible, const glm::vec3& matrix)
{
    GLCall(glUniform3fv(GetUniformLocation(varible), 1, &matrix[0]));
}

void Shader::SetUniformMat4f(const std::string& varible, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(varible), 1, GL_FALSE, &matrix[0][0]));
    //glUniformMatrix4fv(GetUniformLocation(varible), 1, GL_FALSE, glm::value_ptr(matrix));
}

int Shader::GetUniformLocation(const std::string& varible)
{
    if (m_varibles.find(varible) != m_varibles.end())
    {
        return m_varibles[varible];
    }

    int location = glGetUniformLocation(m_programID, varible.c_str());
    if (-1 == location)
    {
        //ASSERT(color != -1);//当shader里未使用该变量时，该变量会被优化掉，因此glGetUniformLocation返回-1，或者其他错误情况也会返回-1
    }
    std::cout << varible << " is:" << location << std::endl;
    m_varibles[varible] = location;//获取program中的统一变量（全局变量）uniform的地址
    return location;
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
        memset(message, 0, length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;//返回shader标识符
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    int result = -1;

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);//将shader对象链接到program对象上，链接一个GPU（可编程顶点处理器）上的可执行对象
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (GL_FALSE == result)
    {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//alloca可以在栈上分配内存
        memset(message, 0, length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << message << std::endl;
        glDeleteProgram(program);
        return false;
    }
    glValidateProgram(program);//验证程序对象

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}