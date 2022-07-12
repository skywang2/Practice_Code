#define GLEW_STATIC 
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
using std::cout;
using std::endl;

int main(int argc, char* argv[])
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);//创建opengl上下文
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        cout << "Error:" << glewGetErrorString(err) << endl;
    }
    cout << "Status: Using GL " << glGetString(GL_VERSION) << endl;//注意函数名
    cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;

    float positions[6] = {
        -0.5f, -0.5f,
        0.f, 0.5f,
        0.5f, -0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);//申请一块buffer并得到他的地址
    glBindBuffer(GL_ARRAY_BUFFER, buffer);//绑定buffer，可能是指定buffer所存储的数据类型
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_DYNAMIC_DRAW);//初始化buffer，单位字节
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //老式opengl用法，已淘汰
        /* glBegin(GL_TRIANGLES);
        glVertex2d(-0.5f, -0.5f);
        glVertex2d(0.f, 0.5f);
        glVertex2d(0.5f, -0.5f);
        glEnd();*/

        //绘图命令
        glDrawArrays(GL_TRIANGLES, 0, 3);//无索引缓冲区
        //glDrawElements(GL_TRIANGLES, 3, NULL);//

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
