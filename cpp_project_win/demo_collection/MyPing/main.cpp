#include <WinSock2.h>
#include <Windows.h>
#include <vector>
#include <string>
#include <memory>
#include <cstdio>
#include "icmp_head.h"
using std::vector;
using namespace std;

const int ICMP_HEADER_SIZE = sizeof(struct icmp);
const int ICMP_PACKET_SIZE = ICMP_HEADER_SIZE + 32;
/*
ICMP协议的"类型码"与"代码"根据不同的情况，各自取不同的值
Ping命令的类型码用到了2个值，分别是0和8，而代码的取值都是0
发送方：
(1)当类型码取值为0，代码取值为0时，表示"请求回显"
(2)当类型码取值为8，代码取值为0时，表示"回送回答"
接收方正好相反
*/
const int ICMP_ECHO_REQUEST  = 0X08;
const int ICMP_ECHO_REPLY = 0X00;

void ProcessError(DWORD dwError)
{
	if (dwError != 0) {
		if (dwError == WSAHOST_NOT_FOUND) {
			printf("Host not found\n");
		}
		else if (dwError == WSANO_DATA) {
			printf("No data record found\n");
		}
		else {
			printf("Function failed with error: %ld\n", dwError);
		}
	}
}

void ping(hostent *remoteHost)
{
	if (nullptr == remoteHost || nullptr == remoteHost->h_addr_list[0])
	{
		return;
	}

	char szBuff[ICMP_PACKET_SIZE] = { 0 };
	icmp *pIcmp = (icmp*)szBuff;

	//创建socket
	SOCKET sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);

	//设置目的地址
	sockaddr_in addr = { 0 };
	addr.sin_family = PF_INET;
	addr.sin_addr.s_addr = *(ULONG*)remoteHost->h_addr_list[0];
	addr.sin_port = htons(0);

	//构造报文
	UINT16 sequence = 0;
	pIcmp->type = ICMP_ECHO_REQUEST;
	pIcmp->code = 0;
	pIcmp->id = (UINT16)GetCurrentProcessId();
	while (sequence < 4)
	{
		pIcmp->sequence = sequence++;
		memcpy((szBuff + ICMP_HEADER_SIZE), )
	}
}

int main(int argc, char* argv[])
{
	vector<string> strArgs;
	for (auto i = argv; i != argv + argc; i++)
	{
		strArgs.push_back(*i);
	}

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	char *hostName = "localhost";
	hostent *remoteHost = nullptr;
	remoteHost = gethostbyname(hostName);
	if (nullptr == remoteHost)
	{
		ProcessError(WSAGetLastError());
		return 1;
	}
	printf("Official name: %s\n", remoteHost->h_name);
	//printf("list of addresses: %s\n", remoteHost->h_addr_list[0]);
	//printf("alias list: %s\n", remoteHost->h_aliases[0]);

	ping(remoteHost);

	WSACleanup();
	return 0;
}
