// oneORtwo.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"



#include <iostream>
#include <stdlib.h>
#include <Windows.h>
using namespace std;

class Solution {
public:
	int kind = 0;
	int jumpFloor(int number) {
		if (number <= 0) {
			return -1;
		}

		if (number == 1 || number == 2) {
			return number;
		}
		else {
			return jumpFloor(number - 1) + jumpFloor(number - 2);
		}
	}

	int jump_real(int number) {
		if (number == 0) {
			kind += 1;
			return kind;
		}

		if (number - 1 >= 0) jump_real(number - 1);
		if (number - 2 >= 0) jump_real(number - 2);

		return kind;
	}

	int jump2(int number) {
		kind = 0;
		kind = jump_real(number);
		return kind;
	}
};

int main() {
	Solution test;
	int n = 1;
	DWORD start_time, end_time, Subtime;
	for (n = 1; n < 42; n++) {

		start_time = GetTickCount();
		cout << "kind1111 = " << test.jumpFloor(n);
		end_time = GetTickCount();
		Subtime = (end_time - start_time);
		cout << "   time = " << Subtime << endl;

		start_time = GetTickCount();
		cout << "kind2222 = " << test.jump2(n);
		end_time = GetTickCount();
		Subtime = (end_time - start_time);
		cout << "   time = " << Subtime << endl;

		cout << endl;
	}


	system("pause");
	return 0;
}
