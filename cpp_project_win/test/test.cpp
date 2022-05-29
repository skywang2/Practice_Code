#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include <Windows.h>
#include "HuaweiJudge.h"
#include "reverse.h"
using namespace std;

const int matM = 3, matN = 2, matP = 4;

class JumpFloorSolution {
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

void matrxMultiply(int matrix1[matM][matN], int matrix2[matN][matP], int result[matM][matP]) {
	int i, j, k;
	for(i = 0; i < matM; i++)
		for (j = 0; j < matP; j++)
			for (k = 0; k < matN; k++) {
				result[i][j] = result[i][j] + matrix1[i][k] * matrix2[k][j];
			}
}

int main()
{
	//3个矩阵相乘
	int matrix1[matM][matN], matrix2[matN][matP], result[matM][matP];

	for (int i = 0; i < matM; i++)//矩阵1
		for (int j = 0; j < matN; j++)
			//cin >> matrix1[i][j];

	for (int i = 0; i < matN; i++)//矩阵2
		for (int j = 0; j < matP; j++)
			//cin >> matrix2[i][j];

	for (int i = 0; i < matN; i++)//矩阵2
		for (int j = 0; j < matP; j++)
			result[i][j] = 0;

	matrxMultiply(matrix1, matrix2, result);

	//跳台阶算法题，来源忘了
	JumpFloorSolution test;
	DWORD start_time, end_time, Subtime;
	for (int n = 1; n < 10; n++) {
		start_time = GetTickCount64();
		/*cout << "kind1111 = " << */test.jumpFloor(n);
		end_time = GetTickCount64();
		Subtime = (end_time - start_time);
		//cout << "   time = " << Subtime << endl;

		start_time = GetTickCount64();
		/*cout << "kind2222 = " << */test.jump2(n);
		end_time = GetTickCount64();
		Subtime = (end_time - start_time);
		//cout << "   time = " << Subtime << endl;
	}
	cout << endl;

	//huawei002
	//HJ2();

	//huawei014
	//int num = 0;
	//cin >> num;
	//HJ14(num);

	//迷宫最短路径，广度遍历
	//BFS();

	//反转链表
	ListNode a1(1);
	ListNode a2(2);
	ListNode a3(3);
	a1.next = &a2;
	a2.next = &a3;
	ReverseList s;
	s.reverseList(&a1);

	//尝试int a{};的写法
	int a{};
	std::cout << "a:" << a << std::endl;

	//读取中间带有\0的字符串
	char buff_zero[] = "{\"class\"=1}\0Content-Disposition: form-data; name=\"file000\"; filename=\"HTTP协议详解.pdf\"  "
		"Content - Type: application / octet - stream\n"
		"{\"class\"=2}\0";
	std::list<std::string> strEmptys;
	size_t buff_count = 0;

	for (int i = 0; i < 2; i++)
	{
		strEmptys.push_back(std::string(buff_zero + buff_count));
		buff_count += strEmptys.back().size() + 1;
	}

	std::string::size_type found_z = strEmptys.back().find_first_of("\n");
	std::string firstPart(strEmptys.back(), found_z + 1);


	return 0;
}

