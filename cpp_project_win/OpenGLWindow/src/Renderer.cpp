#include "Renderer.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

GLFWwindow* g_window = nullptr;

void GLClearError()
{
    //glGetError调用时会返回flag并将其置为GL_NO_ERROR，通过循环将所有flag置为GL_NO_ERROR
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    //通过循环返回所有flag
    while (GLenum error = glGetError())
    {
        std::cout << "[GL_error]"
            << "[" << function << "]"
            << "[" << file << ":" << line << "]"
            << "(0x" << std::hex << error << ")" << std::endl;
        return false;
    }
    return true;
}

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void MouseCallback(GLFWwindow* window)
{

}

void ScrollCallback(GLFWwindow* window)
{
}

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    va.Bind();
    ib.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));//使用顶点索引绘制
}

void Renderer::Clear() const
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}
