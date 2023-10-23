#pragma once
#include <cstdint>
#include <chrono>
#include <string>

#define ICMP_SEND_DATALEN 64		//����ʱ���ݲ��ֵĴ�С
#define ICMP_RECV_BUFFLEN 128		//���ջ���Ĵ�С

#define DEFAULT_INFINITE false		//Ĭ���Ƿ������շ�
#define DEFAULT_SEND_COUNT 4		//Ĭ���շ�����
#define DEFAULT_RECV_TIMEOUT_USEC 10*1000		//���ճ�ʱ����

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
/// ping���������ڼ���������������Ƿ���ͨ���Լ��ӳ١���������Ϣ
/// </summary>
/// <param name="hostAddr">����ip</param>
/// <returns>�Ƿ�������������</returns>
int Ping(const char* hostAddr);
#endif // USE_C_VERSION


/// <summary>
/// ping�����֧࣬��ʹ��icmp���Ľ�����ͨ�Բ��Ժ��ӳ�ʱ�����
/// </summary>
class PingTest
{
public:
	PingTest(const std::string hostAddr);

	/// <summary>
	/// ��ʼ����
	/// </summary>
	/// <returns>0���ɹ���-1��ʧ��</returns>
	int Ping();

	/// <summary>
	/// ����icmp������
	/// </summary>
	/// <param name="sock">�����׽���</param>
	/// <param name="addr">ip��ַ</param>
	/// <param name="ident">��ʶ</param>
	/// <param name="seq">���</param>
	/// <returns>0���ɹ���-1��ʧ��</returns>
	int SendEchoRequest(SOCKET sock, sockaddr_in* addr, uint16_t ident, uint16_t seq);

	/// <summary>
	/// ����icmp��Ӧ����
	/// </summary>
	/// <param name="sock">�����׽���</param>
	/// <param name="ident">��ʶ</param>
	/// <returns>0���ɹ���-1��ʧ��</returns>
	int RecvEchoReply(SOCKET sock, uint16_t ident);

	/// <summary>
	/// ��������У��ͣ��㷨����RFC1071
	/// </summary>
	/// <param name="addr">buff��ַ</param>
	/// <param name="count">buff�ֽ���</param>
	/// <returns>У���</returns>
	uint16_t Checksum(unsigned char* addr, int count);

private:
	std::chrono::system_clock::time_point m_sendTime;
	std::chrono::system_clock::time_point m_recvTime;
	std::string m_host;
	int m_count;//���ʹ���
	bool m_infinite;//�Ƿ��������
	int m_timeout;
};