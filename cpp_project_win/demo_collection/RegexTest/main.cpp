#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <regex>
#include "allocator_test.h"
#include "singleton.h";

//Singleton* logger = Singleton::GetInstance(1, "this is singleton");
void RegextTest(std::string in) {
	
}

int main() {
	TestAllocator1();
	Singleton* x1 = Singleton::GetInstance(1, "this is singleton");
	Singleton* x2 = Singleton::GetInstance(2, "xxxxx");//返回已存在指针
	
	//使用fstream::rebuf()读取缓存内容，使用stringstream进行类型转换
	std::fstream myFile("test.txt", std::ios::in);
	std::stringstream ss;
	std::string in;
	if (myFile.is_open()) {
		ss << myFile.rdbuf();
		in.append(ss.str());
		myFile.close();
	}

	//测试istream_iterator
	std::istream_iterator<std::string> is(std::cin);
	std::istream_iterator<std::string> eof;
	std::vector<std::string> text;
	copy(is, eof, back_inserter(text));

	return 0;
}
