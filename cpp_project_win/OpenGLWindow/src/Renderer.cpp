#include "Renderer.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

void GLClearError()
{
    //glGetError����ʱ�᷵��flag��������ΪGL_NO_ERROR��ͨ��ѭ��������flag��ΪGL_NO_ERROR
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    //ͨ��ѭ����������flag
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