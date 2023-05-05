#include <Windows.h>
#include <iostream>
using namespace std;

HHOOK kKeyboardHook;

//钩子回调函数，KeyboardProc
//对于同一个事件，wParam和lParam中存储的信息是不同的（例如状态+坐标、窗口id+窗口句柄）
LRESULT CALLBACK HookProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		if (wParam == WM_SYSKEYDOWN || wParam == WM_KEYDOWN)
		{
			KBDLLHOOKSTRUCT *p = (KBDLLHOOKSTRUCT *)lParam;

			if (p)
			{
				cout << "char:" << (char)p->vkCode << ", vk:" << p->vkCode << ", scan:" << p->scanCode << endl;
			}
		}
	}

	return nCode;
}

//设置键盘钩子
BOOL HookKeyBoard()
{
	kKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL,	//  low-level keyboard input events，底层键盘事件
		HookProcedure,																		//  回调函数地址
		GetModuleHandle(NULL),														// A handle to the DLL containing the hook procedure 
		NULL																						//线程ID，欲勾住的线程（为0则不指定，全局）
		);

	if (!kKeyboardHook)
	{
		std::cout << "SetWindowsHookEx failed" << std::endl;
		return FALSE;
	}
	else
	{
		std::cout << "SetWindowsHookEx success" << std::endl;

		MSG Msg{};
		while (GetMessage(&Msg, NULL, 0, 0) > 0)//创建事件循环
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		return TRUE;
	}
}


int main(int argc, char* argv[])
{
	HookKeyBoard();
	return 0;
}
