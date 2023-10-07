#pragma once
#include <cstdint>
#include <chrono>
#include <string>

#define ICMP_SEND_DATALEN 64		//发送时数据部分的大小
#define ICMP_RECV_BUFFLEN 128		//接收缓冲的大小

#define DEFAULT_INFINITE false		//默认是否无限收发
#define DEFAULT_SEND_COUNT 4		//默认收发次数
#define DEFAULT_RECV_TIMEOUT_USEC 10*1000		//接收超时秒数

//#define USE_C_VERSION

typedef struct ICMP_ECHO
{
	uint8_t type;
	uint8_t code;
	uint16_t checksum;

	uint16_t ident;
	uint16_t seq;

	// data
	char data[ICMP_SEND_DATALEN];
} ICMP_ECHO_REQ, ICMP_ECHO_REPLY;

#ifdef USE_C_VERSION
/// <summary>
/// ping函数，用于检测与主机的网络是否连通，以及延迟、丢包等信息
/// </summary>
/// <param name="hostAddr">主机ip</param>
/// <returns>是否正常连接主机</returns>
int Ping(const char* hostAddr);
#endif // USE_C_VERSION


/// <summary>
/// ping功能类，支持使用icmp报文进行连通性测试和延迟时间测试
/// </summary>
class PingTest
{
public:
	PingTest(const std::string hostAddr);

	/// <summary>
	/// 开始测试
	/// </summary>
	/// <returns>0：成功；-1：失败</returns>
	int Ping();

	/// <summary>
	/// 发送icmp请求报文
	/// </summary>
	/// <param name="sock">网络套接字</param>
	/// <param name="addr">ip地址</param>
	/// <param name="ident">标识</param>
	/// <param name="seq">序号</param>
	/// <returns>0：成功；-1：失败</returns>
	int SendEchoRequest(SOCKET sock, sockaddr_in* addr, uint16_t ident, uint16_t seq);

	/// <summary>
	/// 接收icmp响应报文
	/// </summary>
	/// <param name="sock">网络套接字</param>
	/// <param name="ident">标识</param>
	/// <returns>0：成功；-1：失败</returns>
	int RecvEchoReply(SOCKET sock, uint16_t ident);

	/// <summary>
	/// 计算网络校验和，算法来自RFC1071
	/// </summary>
	/// <param name="addr">buff地址</param>
	/// <param name="count">buff字节数</param>
	/// <returns>校验和</returns>
	uint16_t Checksum(unsigned char* addr, int count);

private:
	std::chrono::system_clock::time_point m_sendTime;
	std::chrono::system_clock::time_point m_recvTime;
	std::string m_host;
	int m_count;//发送次数
	bool m_infinite;//是否持续发送
	int m_timeout;
};
