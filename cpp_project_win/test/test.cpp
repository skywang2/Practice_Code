#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <iostream>
#include <Windows.h>
using namespace std;

const int matM = 3, matN = 2, matP = 4;

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



    return 0;
}

