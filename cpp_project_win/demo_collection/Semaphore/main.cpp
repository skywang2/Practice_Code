#include <iostream>
#include <Windows.h>

using std::cout;
using std::endl;

void main() {
	HANDLE hSem = 0;
	hSem = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, "MySem");
	if (0 == hSem) {
		cout << "CreateSemaphore" << endl;
		hSem = CreateSemaphore(NULL, 1, 1, "MySem");
	}

	cout << "waiting..." << endl;
	WaitForSingleObject(hSem, INFINITE);
	cout << "process:" << (int)hSem << endl;
	system("pause");
	ReleaseSemaphore(hSem, 1, NULL);

	return;
}
