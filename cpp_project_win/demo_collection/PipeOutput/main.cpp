#include <iostream>
#include <string>
#include <sstream>
#include <Windows.h>
using namespace std;

int main() {
	string rdbuf;
	stringstream ss;

	HANDLE hPipe = CreateNamedPipe(
		"\\\\.\\Pipe\\testPipe",
		PIPE_ACCESS_DUPLEX,
		PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		0,
		0,
		NMPWAIT_WAIT_FOREVER,
		NULL);

	if (INVALID_HANDLE_VALUE == hPipe) {
		cout << "CreateNamedPipe failed." << endl;
		return 1;
	}

	if (!ConnectNamedPipe(hPipe, NULL)) {
		cout << "ConnectNamedPipe failed." << endl;
		return 1;
	}
	else
	{
		cout << "ConnectNamedPipe success." << endl;
	}

	while (cin >> rdbuf) {
		char *buf = new char[rdbuf.size()+1];
		memset(buf, 0, rdbuf.size());
		ss.clear();
		ss << rdbuf;
		ss >> buf;
		//memcpy(buf, rdbuf.c_str(), rdbuf.size());
		DWORD wLen = 0;
		if (!WriteFile(hPipe, buf, rdbuf.size(), &wLen, 0)) {
			delete[] buf;
			continue;
		}
		delete[] buf;
	}

	CloseHandle(hPipe);
	system("pause");
	return 0;
}
