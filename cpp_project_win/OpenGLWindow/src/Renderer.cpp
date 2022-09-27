#include "Renderer.h"
#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

GLFWwindow* g_window = nullptr;
MouseParam* g_mouseParam = nullptr;

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

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
    /*===========
    FPS风格相机，通过鼠标位置参数计算镜头方向
    1.计算鼠标距上一帧的偏移量
    2.把偏移量加到俯仰角和偏航角中
    3.对偏航角、俯仰角设置上下限
    4.计算方向向量
    xoffset->yaw
    yoffset->pitch
    ===========*/
    if (!g_mouseParam) 
    {
        g_mouseParam = new MouseParam;
    }

    //避免鼠标进入窗口第一帧出现剧烈移动，使此时的鼠标位置为初始点
    if (g_mouseParam->firstMove)
    {
        g_mouseParam->lastX = xpos;
        g_mouseParam->lastY = ypos;
        g_mouseParam->firstMove = false;
    }

    //1
    double xoffset = xpos - g_mouseParam->lastX;
    double yoffset = -(ypos - g_mouseParam->lastY);
    g_mouseParam->lastX = xpos;
    g_mouseParam->lastY = ypos;
    //std::cout << "x offset:" << xoffset << ", xpos:" << xpos << ", lastX:" << g_mouseParam->lastX << std::endl;
    //std::cout << "y offset:" << yoffset << ", ypos:" << ypos << ", lastY:" << g_mouseParam->lastY << std::endl;
    //2
    g_mouseParam->yaw += xoffset * g_mouseParam->xsensitivity;
    g_mouseParam->pitch += yoffset * g_mouseParam->ysensitivity;
    //3
    if (g_mouseParam->pitch > 89.0f) { g_mouseParam->pitch = 89.0f; }
    if (g_mouseParam->pitch < -89.0f) { g_mouseParam->pitch = -89.0f; }
    //4
    g_mouseParam->front.x = (float)cos(glm::radians(g_mouseParam->pitch)) * (float)cos(glm::radians(g_mouseParam->yaw));
    g_mouseParam->front.y = (float)sin(glm::radians(g_mouseParam->pitch));
    g_mouseParam->front.z = (float)cos(glm::radians(g_mouseParam->pitch)) * (float)sin(glm::radians(g_mouseParam->yaw));
    g_mouseParam->front = glm::normalize(g_mouseParam->front);
}

void MouseEnterCallback(GLFWwindow* window, int entered)
{
    //std::cout << "entered:" << entered << std::endl;
    if (!g_mouseParam) { return; }
    if (1 == entered) { g_mouseParam->firstMove = true; }
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    //std::cout << "yoffset:" << yoffset << std::endl;
    if (!g_mouseParam) { return; }
    
    double& fov = g_mouseParam->fov;
    double fovMin = 5.0/*500mm镜头*/, fovMax = 114.0/*14mm镜头*/;
    if (fov >= fovMin && fov <= fovMax) { fov -= yoffset; }
    if (fov <= fovMin) { fov = fovMin; }
    if (fov >= fovMax) { fov = fovMax; }
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
