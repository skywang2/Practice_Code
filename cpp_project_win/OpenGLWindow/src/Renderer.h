#ifndef SRC_RENDERER_H
#define SRC_RENDERER_H

#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#else
#define ASSERT(x) (x)
#endif
//GLCall���������ӡ��debug�ϵ�ĵ��ú�
//����ȱ���޷�����if��Ԫʽ��Ҳ�޷���������ֵ��bool��
//#x��xת�����ַ�����
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__))
//glDebugMessageCallback//����glGetError�����ṩ�����ı���Ϣ

struct GLFWwindow;

void GLClearError();//����Ƿ���ڴ��������������
bool GLLogCall(const char* function, const char* file, int line);//��ÿ����������flag
void ProcessInput(GLFWwindow* window);//����״̬��������

#endif // !SRC_RENDERER_H