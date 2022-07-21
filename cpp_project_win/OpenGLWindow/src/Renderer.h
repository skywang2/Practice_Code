#ifndef SRC_RENDERER_H
#define SRC_RENDERER_H

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

void GLClearError();//检查是否存在错误，如果有则阻塞
bool GLLogCall(const char* function, const char* file, int line);//在每个函数后检查flag
void ProcessInput(GLFWwindow* window);//按键状态处理函数

#endif // !SRC_RENDERER_H
