#ifndef SRC_RENDERER_H
#define SRC_RENDERER_H
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

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

struct GLFWwindow;
extern GLFWwindow* g_window;//全局变量，窗口对象指针

//鼠标事件相关参数记录
struct MouseParam
{
    bool firstMove;//第一次进入窗口
    double xsensitivity;
    double ysensitivity;
    double lastX;
    double lastY;
    double yaw;
    double pitch;
    glm::vec3 front;

    MouseParam()
        : firstMove(true),
        xsensitivity(0.20),
        ysensitivity(0.05),
        yaw(-90.0),
        pitch(0.0),
        front()
    {
    }
};
extern MouseParam* g_mouseParam;

void GLClearError();//检查是否存在错误，如果有则阻塞
bool GLLogCall(const char* function, const char* file, int line);//在每个函数后检查flag
void ProcessInput(GLFWwindow* window);//键盘事件处理函数
void MouseCallback(GLFWwindow * window, double xpos, double ypos);//鼠标事件
void MouseEnterCallback(GLFWwindow* window, int entered);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);//滚轮事件

class Renderer
{
public:
    Renderer();
    ~Renderer();

    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    void Clear() const;
private:

};



#endif //SRC_RENDERER_H
