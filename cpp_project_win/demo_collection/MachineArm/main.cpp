#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "TCP2MA.hpp"

int main(int argc, char* argv[])
{
	static TCP2MA ma;
	std::string ip("192.168.1.64");
	std::string request, response;
	std::vector<std::string> commands;

	//读文件
	std::fstream file;
	std::string line;

	file.open("Machine_Arm_Commands.txt", std::fstream::in);
	if (file.is_open())
	{
		while (std::getline(file, line))
		{
			line = "HOSTCTRL_REQUEST " + line + "\r\n";
			commands.push_back(line.c_str());
		}
		file.close();
	}
	commands.insert(commands.begin(), "CONNECT Robot_access Keep-Alive:" + std::to_string(commands.size()) + "\r\n");

	//连接、发命令
	if (ma.connect(ip))
	{
		for (auto& cmd : commands)
		{
			ma.send(request, response);
		}
		if (!ma.close())
		{
			std::cout << "断开连接失败";
		}
	}
	else
	{
		std::cout << "连接失败";
	}
}
