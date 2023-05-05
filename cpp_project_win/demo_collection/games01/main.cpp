#include <stdio.h>
#include <locale.h>
#include <Windows.h>

//设置边框
void SetBuffBox(WCHAR* const buff, const int &width=80, const int &hight=25)
{
	if (buff == NULL || width <= 0 || hight <= 0)
	{
		//todo:output error
		return;
	}

	const int buffSize = width * hight;
	const int &row = hight;
	const int &col = width;
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			if (0 == i || row - 1 == i || 0 == j || col - 2 == j)//上下左右边框
			{
				*(buff + row * i + j) = L'■';//□
			}
			else
			{
				*(buff + row * i + j) = L'□';//在每行末尾加\n
			}
			//if (col -1 == j)
			//{
			//	*(buff + row * i + j) = L'\n';
			//}
		}
	}//for
	return;
}

int SetCmdWindowMode(int w, int h)
{
	HANDLE hSTD = NULL;
	COORD size = { 0, 0 };
	SMALL_RECT rect = { 0 };

	size = { w, h };
	rect = { 1, 1, w, h };
	hSTD = GetStdHandle(STD_OUTPUT_HANDLE);
	if (NULL == hSTD) return -1;
	if (!SetConsoleScreenBufferSize(hSTD, size)) return GetLastError();
	if (!SetConsoleWindowInfo(hSTD, true, &rect))return GetLastError();
	return ERROR_SUCCESS;
}

int main()
{
	HANDLE hSTD = NULL;
	HANDLE hBuffer = NULL;
	COORD coord = { 0, 0 };
	CONSOLE_CURSOR_INFO cci = { 0 };
	DWORD bytes = 0;
	const int width = 50;
	const int higth = 20;
	const int buffSize = width * higth;
	WCHAR gameBuffer[buffSize] = { 0 };
	WCHAR screenBuffer[buffSize] = { 0 };	//定义缓冲区
	SMALL_RECT smallRect = { 0 };
	int ret = 0;

	//设置locale信息
	setlocale(LC_ALL, "chs");
	//获取stdout句柄
	hSTD = GetStdHandle(STD_OUTPUT_HANDLE);
	//设置窗口大小
	//smallRect = { 0, 0, width-1, higth-1 };
	//if (!SetConsoleWindowInfo(hSTD, FALSE, &smallRect))
	//{
	//	ret = GetLastError();
	//}
	//system("cls");
	if ((ret = SetCmdWindowMode(width, higth)) != ERROR_SUCCESS)
	{
		//可能是因为系统安全软件原因导致hSTD的GENERIC_WRITE权限被限制导致
	}
	//创建新缓冲区
	/*hBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL);
	//设置新缓冲区为活动缓冲区
	SetConsoleActiveScreenBuffer(hBuffer);
	//隐藏两个缓冲区的光标
	cci.bVisible = 0;
	cci.dwSize = 1;
	SetConsoleCursorInfo(hSTD, &cci);
	SetConsoleCursorInfo(hBuffer, &cci);

	//双缓冲处理显示
	while (true)
	{
		//system("cls");
		//在gamebuffer中设置边框等位置
		memset(gameBuffer, 0, sizeof(WCHAR) * buffSize);
		//SetBuffBox(gameBuffer, width, higth);
		
		printf("%ls", gameBuffer);//输出到stdout
		memset(screenBuffer, 0, sizeof(char) * buffSize);
		if (!ReadConsoleOutputCharacter(hSTD, screenBuffer, buffSize, coord, &bytes))
		{
			continue;
		}
		Sleep(10);
		if (!WriteConsoleOutputCharacter(hBuffer, screenBuffer, buffSize, coord, &bytes))
		{
			continue;
		}
	}//while
	*/

	for (int i = 0; i < 10; ++i)
	{
		printf("%ls", L"中");
	}

	//system("pause");
	return 0;
}
