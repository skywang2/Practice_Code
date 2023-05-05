#pragma once
#include <cstring>
#include <iostream>

void HJ5() {
	char in[1024];
	memset(in, 0, 1024);
	while (std::cin >> in) {
		int size = strlen(in);
		int out = 0;
		for (int i = 2; i < size; i++) {
			int rank = size - i - 1;//阶数

			int bit=-1;//每一位数值
			if (in[i] >= 'a' && in[i] <= 'f') {
				bit = 10 + in[i] - 'a';
			}
			else if (in[i] >= 'A' && in[i] <= 'F') {
				bit = 10 + in[i] - 'A';
			}
			else if (in[i] >= '0' && in[i] <= '9') {
				bit = in[i] - '0';
			}

			out += bit * 16 ^ rank;
		}
		std::cout << out << std::endl;
	}
}
