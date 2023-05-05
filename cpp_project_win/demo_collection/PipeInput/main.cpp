#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;

int main() {
	char buf[256] = { 0 };
	DWORD rLen = 0;

	if (!WaitNamedPipe("\\\\.\\Pipe\\testPipe", NMPWAIT_WAIT_FOREVER)) {
		cout << "WaitNamedPipe failed." << endl;
		return 1;
	}
	else
	{
		cout << "WaitNamedPipe success." << endl;
	}

	HANDLE hPipe = CreateFile(					//创建管道文件，即链接管道  
		"\\\\.\\Pipe\\testPipe",				//管道名称  
		GENERIC_READ | GENERIC_WRITE,		//文件模式  
		0,															//是否共享  
		NULL,													//指向一个SECURITY_ATTRIBUTES结构的指针  
		OPEN_EXISTING,									//创建参数  
		FILE_ATTRIBUTE_NORMAL,				//文件属性，NORMAL为默认属性  
		NULL);

	if (INVALID_HANDLE_VALUE == hPipe) {
		cout << "CreateFile failed." << endl;
		return 1;
	}

	while (ReadFile(hPipe, buf, 256, &rLen, NULL)) {
		cout << buf << endl;
		memset(buf, 0, 256);
	}

	CloseHandle(hPipe);
	system("pause");
	return 0;
}
