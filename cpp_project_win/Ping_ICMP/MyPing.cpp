#include "MyPing.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string.h>
#include <iostream>

#define RECV_TIMEOUT_SEC 10
#define RECV_TIMEOUT_USEC 10 * 1000

/// <summary>
/// 计算网络校验和，算法来自RFC1071
/// </summary>
/// <param name="addr">buff地址</param>
/// <param name="count">buff字节数</param>
/// <returns></returns>
uint16_t checksum(unsigned char* addr, int count)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     */
    uint32_t sum = 0;

    while (count > 1) {
        /*  This is the inner loop */
        sum += *(unsigned short*)addr++;//等价于*（addr++），先取addr当前值，最后加1
        addr++;//因为是short，所以原本char的指针需要移动两次
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if (count > 0)
        sum += *(unsigned char*)addr;

    /*  Fold 32-bit sum to 16 bits */
    while (sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

/// <summary>
/// 发送icmp请求，发送一段hello world
/// </summary>
/// <param name="sock">套接字</param>
/// <param name="addr">目标主机ip结构</param>
/// <param name="ident">标识</param>
/// <param name="seq">序号</param>
/// <returns></returns>
int SendEchoRequest(SOCKET sock, sockaddr_in* addr, uint16_t ident, uint16_t seq)
{
    ICMP_ECHO_REQ icmp;
    memset(&icmp, 0, sizeof(icmp));

    icmp.type = 8;
    icmp.code = 0;
    icmp.ident = htons(ident);
    icmp.seq = htons(seq);

    //data
    char payload[64] = "hello world";
    //memset(payload, 0, sizeof(payload));
    memcpy_s(icmp.data, sizeof(icmp.data), payload, sizeof(payload));
    //strncpy_s(icmp.data, payload, strlen(payload));    
    icmp.checksum = checksum((unsigned char*)&icmp, sizeof(icmp));

    //send
    int bytes = sendto(sock, (const char*)&icmp, sizeof(icmp), 0, (sockaddr*)addr, sizeof(*addr));
    if (SOCKET_ERROR == bytes)
    {
        int err = WSAGetLastError();
        std::cout << "sendto failed, error: " << err << std::endl;
        return -1;
    }

    return 0;
}

int RecvEchoReply(SOCKET sock, uint16_t ident)
{
    char buffer[1024];
    ICMP_ECHO_REQ* icmp = nullptr;
    //sockaddr_in hostAddr;

    //接收IP报文
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, nullptr, nullptr);
    if (SOCKET_ERROR == bytes) {
        int err = WSAGetLastError();
        std::cout << "recvfrom failed, error: " << err << std::endl;
        return -1;
    }

    //提取ICMP报文
    int ip_header_len = (buffer[0] & 0xf) << 2;
    icmp = (ICMP_ECHO_REQ*)(buffer + ip_header_len);

    //读取icmp头部信息
    uint16_t tmpident = ntohs(icmp->ident);
    uint16_t tmpseq = ntohs(icmp->seq);
    std::cout << "reply:" << icmp->data << std::endl;

    return 0;
}

//可以将发送数据替换为本机发送时间，再记录收到返回报文的时间，即可计算报文延迟
int Ping(const char* chHostAddr)
{
    //创建socket
    SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (INVALID_SOCKET == sock)
    {
        std::cout << "socket error: " << WSAGetLastError() << std::endl;
        return -1;
    }
    
    //ip地址转为结构体
    sockaddr_in hostAddr;
    memset(&hostAddr, 0, sizeof(hostAddr));
    hostAddr.sin_family = AF_INET;
    inet_pton(AF_INET, chHostAddr, &hostAddr.sin_addr);

    //设置收发超时时间
    int timeout = RECV_TIMEOUT_USEC;
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    if (ret == -1) {
        std::cout << "set socket option" << std::endl;
        return 1;
    }

    //循环发送、接受报文，处理返回信息
    int count = 4;
    int cur = 0;
    bool infinite = false;
    int seq = 1;
    uint16_t ident = (uint16_t)GetCurrentProcessId();
    while (1)
    {
        //send
        if (0 != SendEchoRequest(sock, &hostAddr, ident, seq))
        {
            std::cout << "send failed" << std::endl;
        }
        //recv
        if (0 != RecvEchoReply(sock, ident))
        {
            std::cout << "recv failed" << std::endl;
        }

        //calculate

        seq++;
        cur++;
        if (false == infinite && cur >= count)
        {
            break;
        }
    }

	return 0;
}
