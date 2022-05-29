#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include <Windows.h>
using namespace std;

const int matM = 3, matN = 2, matP = 4;

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
	//3¸ö¾ØÕóÏà³Ë
	int matrix1[matM][matN], matrix2[matN][matP], result[matM][matP];

	for (int i = 0; i < matM; i++)//¾ØÕó1
		for (int j = 0; j < matN; j++)
			//cin >> matrix1[i][j];

	for (int i = 0; i < matN; i++)//¾ØÕó2
		for (int j = 0; j < matP; j++)
			//cin >> matrix2[i][j];

	for (int i = 0; i < matN; i++)//¾ØÕó2
		for (int j = 0; j < matP; j++)
			result[i][j] = 0;

	matrxMultiply(matrix1, matrix2, result);

	//ÌøÌ¨½×Ëã·¨Ìâ£¬À´Ô´ÍüÁË
	Solution test;
	int n = 1;
	DWORD start_time, end_time, Subtime;
	for (n = 1; n < 10; n++) {
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

		cout << endl;
	}


    return 0;
}

