#include <cstring>
#include <iostream>
#include <Windows.h>
using namespace std;

int main() {
	DWORD exitCode = -1;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	WCHAR exePath[MAX_PATH] = L"argc.exe a b c d";
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));

	//创建进程
	CreateProcess(
		NULL,
		exePath,
		NULL,
		NULL,
		false,
		NULL, //CREATE_NEW_CONSOLE,
		NULL,
		NULL,
		&si,
		&pi);

	cout << "parent finish." << endl;

	WaitForSingleObject(pi.hProcess, INFINITE);
	GetExitCodeProcess(pi.hProcess, &exitCode);

	// Close process and thread handles. 
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	system("pause");

	return 0;
}
