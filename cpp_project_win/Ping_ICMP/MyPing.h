#pragma once
#include <cstdint>

//#define PingRecv

typedef struct ICMP_ECHO
{
	uint8_t type;
	uint8_t code;
	uint16_t checksum;

	uint16_t ident;
	uint16_t seq;

	// data
	char data[64];
} ICMP_ECHO_REQ, ICMP_ECHO_REPLY;

class PingTest
{
public:
	PingTest() {};

private:

};


/// <summary>
/// ping函数，用于检测与主机的网络是否连通，以及延迟、丢包等信息
/// </summary>
/// <param name="hostAddr">主机ip</param>
/// <returns>是否正常连接主机</returns>
int Ping(const char* hostAddr);
