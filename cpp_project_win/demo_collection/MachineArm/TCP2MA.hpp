#ifndef TCP2MA_H
#define TCP2MA_H

#include <string.h>
#include <stdio.h>
#include <WinSock2.h>
//#include <Windows.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")

//#define _WINSOCK_DEPRECATED_NO_WARNINGS

struct HEADER_REQUEST
{
	unsigned char identifyer[4];//4，识别子
	//数据大小
	unsigned short header_size;//6，标头部大小
	unsigned short data_size;//8，数据部大小
	//预约1/处理区分
	unsigned char reserve1;//9，预约1
	unsigned char process_part;//10，处理区分
	unsigned char ACK;//11，ACK
	unsigned char request_id;//12，请求ID

	unsigned char data_part_num;//16，数据块编号
	unsigned char reserve2[8];//24，预约2
	//子标头
	unsigned short commond_num;//26，命令编号
	unsigned short data_arrange_num;//28，数据的排列编号
	unsigned char unit_num;//29，单元编号
	unsigned char process;//30，处理（要求时）
	unsigned short padding;//32，Padding
};

struct HEADER_RESPONSE
{
	unsigned char identifyer[4];//4，识别子
	//数据大小
	unsigned short header_size;//6，标头部大小
	unsigned short data_size;//8，数据部大小
	//预约1/处理区分
	unsigned char reserve1;//9，预约1
	unsigned char process_part;//10，处理区分
	unsigned char ACK;//11，ACK
	unsigned char request_id;//12，请求ID

	unsigned char data_part_num;//16，数据块编号
	unsigned char reserve2[8];//24，预约2
	//子标头
	unsigned char process;//25，处理（应答时）
	unsigned char state;//26，状态
	unsigned char attached_state_size;//27，附加状态大小
	unsigned char padding1;//28，Padding
	unsigned short attached_state;//30，附加状态
	unsigned short padding2;//32，Padding
};

class TCP2MA
{
public:
	TCP2MA();
	~TCP2MA();

	//Ethernet服务器功能
	bool connect(const std::string& serverIP);
	bool close();
	bool send(const std::string& request, std::string& response);//tcp，80

	//Ethernet服务器功能
	HEADER_REQUEST createRequest(int p1, int p2);//构造发送数据
	HEADER_RESPONSE parseResponse(void* data, int size);//解析返回数据
	bool send(const HEADER_REQUEST& request, HEADER_RESPONSE& response);//udp，10040

private:
	static const int BUFF_SIZE = 1024;
	SOCKET clientSocket;
};

TCP2MA::TCP2MA()
{
	WSADATA wsaData;//socket版本2.2
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		std::cout << "WSAStartup failed";
		return;
	}
	if (2 != LOBYTE(wsaData.wVersion) || 2 != HIBYTE(wsaData.wHighVersion))
	{
		std::cout << "WSAStartup version error";
		WSACleanup();
		return;
	}
}

TCP2MA::~TCP2MA()
{
	closesocket(clientSocket);

	WSACleanup();
}

bool TCP2MA::connect(const std::string& serverIP)
{
	//socket
	clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == clientSocket)
	{
		printf("[%s:%d], create socket failed, error:%d", __FUNCTION__, __LINE__, WSAGetLastError());
		return false;
	}

	//ip/port
	sockaddr_in addrAskIP = { 0 };//连接设备80端口
	addrAskIP.sin_family = AF_INET;
	addrAskIP.sin_addr.s_addr = inet_addr(serverIP.data());
	addrAskIP.sin_port = htons(80);

	//connect
	if (0 != ::connect(clientSocket, (sockaddr*)&addrAskIP, sizeof(sockaddr)))
	{
		printf("[%s:%d], connect dev port 80 failed, error:%d", __FUNCTION__, __LINE__, WSAGetLastError());
		closesocket(clientSocket);
		return false;
	}

	return true;
}

bool TCP2MA::close()
{
	return (0 == closesocket(clientSocket));
}

//tcp，80
bool TCP2MA::send(const std::string& request, std::string& response)
{
	int ret = 0;
	char data[BUFF_SIZE];
	memset(data, 0, sizeof(data));

	strncpy_s(data, sizeof(data), request.c_str(), request.size());
	std::cout << "send request:" << data;
	ret = ::send(clientSocket, data, strlen(data), 0);//发送命令
	std::cout << "send data length:" << ret;

	response.clear();
	memset(data, 0, sizeof(data));
	ret = ::recv(clientSocket, data, sizeof(data), 0);//接收回复
	std::cout << "recv data length:" << ret;
	response.append(data);
	std::cout << "recv response:%1" << data;

	return true;
}

bool TCP2MA::send(const HEADER_REQUEST& request, HEADER_RESPONSE& response)
{
	char content[BUFF_SIZE];
	memset(content, 0, sizeof(content));


	return true;
}

#endif
