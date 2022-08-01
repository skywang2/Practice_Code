#include "Shader.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include "GL/glew.h"
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
    GLCall(glUniform1i(GetUniformLocation(varible), v1));
}

void Shader::SetUniform4f(const std::string& varible, float v1, float v2, float v3, float v4)
{
    GLCall(glUniform4f(GetUniformLocation(varible), v1, v2, v3, v4));
}

void Shader::SetUniformMat4f(const std::string& varible, const glm::mat4& matrix)
{
    GLCall(glUniformMatrix4fv(GetUniformLocation(varible), 1, GL_FALSE, &matrix[0][0]));
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
        //ASSERT(color != -1);//��shader��δʹ�øñ���ʱ���ñ����ᱻ�Ż��������glGetUniformLocation����-1�����������������Ҳ�᷵��-1
    }
    std::cout << varible << " is:" << location << std::endl;
    m_varibles[varible] = location;//��ȡprogram�е�ͳһ������ȫ�ֱ�����uniform�ĵ�ַ
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
        if (line.find("#shader") != std::string::npos)//����#shader
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
    unsigned int id = glCreateShader(type);//����shader����
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);//��shaderԴ��
    glCompileShader(id);//����shaderԴ��

    //�����飬��־
    int result = -1;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (GL_FALSE == result)
    {
        int length = 0;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//alloca������ջ�Ϸ����ڴ�
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;//����shader��ʶ��
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
    int result = -1;

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);//��shader�������ӵ�program�����ϣ�����һ��GPU���ɱ�̶��㴦�������ϵĿ�ִ�ж���
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (GL_FALSE == result)
    {
        int length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));//alloca������ջ�Ϸ����ڴ�
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << message << std::endl;
        glDeleteProgram(program);
        return false;
    }
    glValidateProgram(program);//��֤�������

    glDetachShader(program, vs);
    glDetachShader(program, fs);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}