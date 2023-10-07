#include <string.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "MyPing.h"

#ifdef USE_C_VERSION

std::chrono::system_clock::time_point sendTime;
std::chrono::system_clock::time_point recvTime;

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
    char payload[64] = "Hi";
    //memset(payload, 0, sizeof(payload));
    memcpy_s(icmp.data, sizeof(icmp.data), payload, sizeof(payload));
    //strncpy_s(icmp.data, payload, strlen(payload));    
    icmp.checksum = checksum((unsigned char*)&icmp, sizeof(icmp));

    //send，不能把数据部分和报文头分开发送，这样相当于发了两次icmp报文，数据部分的报文会因为icmp格式不正确而无法被服务端正确识别，应用层的报文或许可以分开发送
    int bytes = sendto(sock, (const char*)&icmp, sizeof(icmp), 0, (sockaddr*)addr, sizeof(*addr));
    sendTime = std::chrono::system_clock::now();//记录发送时间
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
    char buffer[256];
    ICMP_ECHO_REQ* icmp = nullptr;
    //sockaddr_in hostAddr;

    //接收IP报文
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, nullptr, nullptr);
    recvTime = std::chrono::system_clock::now();//记录发送时间
    if (SOCKET_ERROR == bytes) {
        int err = WSAGetLastError();
        std::cout << "recvfrom failed, error: " << err << std::endl;
        return -1;
    }

    //记录接收时间，转换为微秒，以微秒为单位
    auto usedTime = recvTime - sendTime;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(usedTime);

    //提取ICMP报文
    int ip_header_len = (buffer[0] & 0xf) << 2;//报文长度必须为4的整数倍，报文头中记录了这个倍数
    icmp = (ICMP_ECHO_REQ*)(buffer + ip_header_len);

    //读取icmp头部信息
    uint16_t tmpident = ntohs(icmp->ident);
    uint16_t tmpseq = ntohs(icmp->seq);
    printf("reply:%s, time:%lldms\n", icmp->data, milliseconds.count());
    //std::cout << "reply:" << icmp->data << ", time:" << milliseconds.count() << "ms" << std::endl;

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
    sockaddr_in hostAddr = {
        .sin_family = AF_INET,//结构体初始化的一种写法
        .sin_port = 0,
        .sin_addr = {0},
        .sin_zero = {0}
    };
    //memset(&hostAddr, 0, sizeof(hostAddr));
    //hostAddr.sin_family = AF_INET;
    inet_pton(AF_INET, chHostAddr, &hostAddr.sin_addr);

    //设置收发超时时间
    int timeout = DEFAULT_RECV_TIMEOUT_USEC;
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    if (ret == -1) {
        std::cout << "set socket option error" << std::endl;
        closesocket(sock);
        return -1;
    }

    //循环发送、接受报文，处理返回信息
    int count = 4;//需要发送报文总数
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
        count--;
        if (false == infinite && count <= 0)
        {
            break;
        }
    }

    closesocket(sock);
	return 0;
}
#endif // USE_C_VERSION

//封装成类

PingTest::PingTest(const std::string hostAddr)
    :m_host(hostAddr),
    m_count(DEFAULT_SEND_COUNT),
    m_infinite(DEFAULT_INFINITE),
    m_timeout(DEFAULT_RECV_TIMEOUT_USEC)
{
}

int PingTest::Ping()
{
    //创建socket
    SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (INVALID_SOCKET == sock)
    {
        std::cout << "socket error: " << WSAGetLastError() << std::endl;
        return -1;
    }

    //ip地址转为结构体
    sockaddr_in hostAddr = {
        .sin_family = AF_INET,//结构体初始化的一种写法
        .sin_port = 0,
        .sin_addr = {0},
        .sin_zero = {0}
    };
    inet_pton(AF_INET, m_host.c_str(), &hostAddr.sin_addr);

    //设置收发超时时间
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&m_timeout, sizeof(m_timeout));
    if (ret == -1) {
        std::cout << "set socket option error" << std::endl;
        closesocket(sock);
        return -1;
    }

    //发送、接收
    m_count = DEFAULT_SEND_COUNT;
    uint16_t seq = 1;
    uint16_t ident = (uint16_t)GetCurrentProcessId();
    while (1)
    {
        if (0 != SendEchoRequest(sock, &hostAddr, ident, seq))
        {
            std::cout << "send failed" << std::endl;
            goto next;
        }
        if (0 != RecvEchoReply(sock, ident))
        {
            std::cout << "recv failed" << std::endl;
        }

next:
        seq++;
        m_count--;
        if (false == m_infinite && m_count <= 0)
        {
            break;
        }
    }//while

    closesocket(sock);
    return 0;
}

int PingTest::SendEchoRequest(SOCKET sock, sockaddr_in* addr, uint16_t ident, uint16_t seq)
{
    ICMP_ECHO_REQ icmp;
    memset(&icmp, 0, sizeof(icmp));

    icmp.type = 8;
    icmp.code = 0;
    icmp.ident = htons(ident);
    icmp.seq = htons(seq);

    //data
    char payload[ICMP_SEND_DATALEN] = "Hi";
    memcpy_s(icmp.data, sizeof(icmp.data), payload, sizeof(payload));
    icmp.checksum = Checksum((unsigned char*)&icmp, sizeof(icmp));

    //send，不能把数据部分和报文头分开发送，这样相当于发了两次icmp报文，数据部分的报文会因为icmp格式不正确而无法被服务端正确识别，应用层的报文或许可以分开发送
    int bytes = sendto(sock, (const char*)&icmp, sizeof(icmp), 0, (sockaddr*)addr, sizeof(*addr));
    m_sendTime = std::chrono::system_clock::now();//记录发送时间
    if (SOCKET_ERROR == bytes)
    {
        int err = WSAGetLastError();
        std::cout << "sendto failed, error: " << err << std::endl;
        return -1;
    }

    return 0;
}

int PingTest::RecvEchoReply(SOCKET sock, uint16_t ident)
{
    char buffer[ICMP_RECV_BUFFLEN];
    ICMP_ECHO_REQ* icmp = nullptr;
    memset(buffer, 0, sizeof(buffer));

    //接收IP报文
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, nullptr, nullptr);
    m_recvTime = std::chrono::system_clock::now();//记录发送时间
    if (SOCKET_ERROR == bytes) {
        int err = WSAGetLastError();
        std::cout << "recvfrom failed, error: " << err << std::endl;
        return -1;
    }

    //记录接收时间，转换为微秒，以微秒为单位
    auto usedTime = m_recvTime - m_sendTime;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(usedTime);

    //提取icmp报文
    int ip_header_len = (buffer[0] & 0xf) << 2;//报文长度必须为4的整数倍，报文头中记录了这个倍数
    icmp = (ICMP_ECHO_REQ*)(buffer + ip_header_len);

    //读取icmp头部信息
    uint16_t tmpident = ntohs(icmp->ident);
    uint16_t tmpseq = ntohs(icmp->seq);
    std::cout << "from " << m_host \
        << ", ident:" << tmpident \
        << ", seq:" << tmpseq \
        << ", reply:" << icmp->data \
        << ", time:" << milliseconds.count() << "ms" \
        << std::endl;
    //printf("ident:%d, seq:%d, reply:%s, time:%lldms\n", tmpident, tmpseq, icmp->data, milliseconds.count());

    return 0;
}

uint16_t PingTest::Checksum(unsigned char* addr, int count)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     */
    uint32_t sum = 0;

    while (count > 1) {
        /*  This is the inner loop */
        sum += *(unsigned short*)addr;
        addr += 2;//因为是short，所以原本char的指针需要移动两次
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
