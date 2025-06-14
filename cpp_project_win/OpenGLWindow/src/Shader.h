#pragma once
#include <string>
#include <unordered_map>
#include <map>
#include "glm/glm.hpp"

struct ShaderProgramSource
{
	std::string vertexShader;
	std::string fragmentShader;
};

class Shader
{
private:
    enum class ShaderType
    {
        NONE = -1, VERTEX, FRAGMENT
    };

    unsigned int m_programID;
    std::map<const std::string, int> m_varibles;
    std::string m_filepath;

public:
	Shader(const std::string& filepath);//all in one
    Shader(const std::string& vertex, const std::string& fragment);//vertex + fragment
    Shader(const std::string& vertex, const std::string& geometry, const std::string& fragment);//vertex + geometry + fragment
	~Shader();

    void Bind() const;
    void Unbind() const;

    //uniform
    void SetUniform1i(const std::string& varible, int v1);
    void SetUniform1f(const std::string& varible, float v1);
    void SetUniform3f(const std::string& varible, float v1, float v2, float v3);
    void SetUniform4f(const std::string& varible, float v1, float v2, float v3, float v4);
    void SetUniformVec3f(const std::string& varible, const glm::vec3& matrix);
    void SetUniformMat4f(const std::string& varible, const glm::mat4& matrix);

private:
    int GetUniformLocation(const std::string& varible);
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);//传入shader源码字符串，构造着色器的公共代码
    unsigned int CreateShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader);//有几何着色器
};

