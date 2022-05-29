// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define M 3
#define N 2
#define P 4

void matrxMultiply(int matrix1[M][N], int matrix2[N][P], int result[M][P]) {
	int i, j, k;
	for(i = 0; i < M; i++)
		for (j = 0; j < P; j++)
			for (k = 0; k < N; k++) {
				result[i][j] = result[i][j] + matrix1[i][k] * matrix2[k][j];
			}
}

int main()
{
	int i = 0, j = 0;
	int matrix1[M][N], matrix2[N][P], result[M][P];

	for (i = 0; i < M; i++)//矩阵1
		for (j = 0; j < N; j++)
			cin >> matrix1[i][j];

	for (i = 0; i < N; i++)//矩阵2
		for (j = 0; j < P; j++)
			cin >> matrix2[i][j];

	for (i = 0; i < N; i++)//矩阵2
		for (j = 0; j < P; j++)
			result[i][j] = 0;

	matrxMultiply(matrix1, matrix2, result);

    return 0;
}

