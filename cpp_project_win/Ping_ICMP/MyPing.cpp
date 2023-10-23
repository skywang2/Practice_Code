#include <string.h>
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "MyPing.h"

#ifdef USE_C_VERSION

std::chrono::system_clock::time_point sendTime;
std::chrono::system_clock::time_point recvTime;

/// <summary>
/// ��������У��ͣ��㷨����RFC1071
/// </summary>
/// <param name="addr">buff��ַ</param>
/// <param name="count">buff�ֽ���</param>
/// <returns></returns>
uint16_t checksum(unsigned char* addr, int count)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     */
    uint32_t sum = 0;

    while (count > 1) {
        /*  This is the inner loop */
        sum += *(unsigned short*)addr++;//�ȼ���*��addr++������ȡaddr��ǰֵ������1
        addr++;//��Ϊ��short������ԭ��char��ָ����Ҫ�ƶ�����
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
/// ����icmp���󣬷���һ��hello world
/// </summary>
/// <param name="sock">�׽���</param>
/// <param name="addr">Ŀ������ip�ṹ</param>
/// <param name="ident">��ʶ</param>
/// <param name="seq">���</param>
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

    //send�����ܰ����ݲ��ֺͱ���ͷ�ֿ����ͣ������൱�ڷ�������icmp���ģ����ݲ��ֵı��Ļ���Ϊicmp��ʽ����ȷ���޷����������ȷʶ��Ӧ�ò�ı��Ļ������Էֿ�����
    int bytes = sendto(sock, (const char*)&icmp, sizeof(icmp), 0, (sockaddr*)addr, sizeof(*addr));
    sendTime = std::chrono::system_clock::now();//��¼����ʱ��
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

    //����IP����
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, nullptr, nullptr);
    recvTime = std::chrono::system_clock::now();//��¼����ʱ��
    if (SOCKET_ERROR == bytes) {
        int err = WSAGetLastError();
        std::cout << "recvfrom failed, error: " << err << std::endl;
        return -1;
    }

    //��¼����ʱ�䣬ת��Ϊ΢�룬��΢��Ϊ��λ
    auto usedTime = recvTime - sendTime;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(usedTime);

    //��ȡICMP����
    int ip_header_len = (buffer[0] & 0xf) << 2;//���ĳ��ȱ���Ϊ4��������������ͷ�м�¼���������
    icmp = (ICMP_ECHO_REQ*)(buffer + ip_header_len);

    //��ȡicmpͷ����Ϣ
    uint16_t tmpident = ntohs(icmp->ident);
    uint16_t tmpseq = ntohs(icmp->seq);
    printf("reply:%s, time:%lldms\n", icmp->data, milliseconds.count());
    //std::cout << "reply:" << icmp->data << ", time:" << milliseconds.count() << "ms" << std::endl;

    return 0;
}

//���Խ����������滻Ϊ��������ʱ�䣬�ټ�¼�յ����ر��ĵ�ʱ�䣬���ɼ��㱨���ӳ�
int Ping(const char* chHostAddr)
{
    //����socket
    SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (INVALID_SOCKET == sock)
    {
        std::cout << "socket error: " << WSAGetLastError() << std::endl;
        return -1;
    }
    
    //ip��ַתΪ�ṹ��
    sockaddr_in hostAddr = {
        .sin_family = AF_INET,//�ṹ���ʼ����һ��д��
        .sin_port = 0,
        .sin_addr = {0},
        .sin_zero = {0}
    };
    //memset(&hostAddr, 0, sizeof(hostAddr));
    //hostAddr.sin_family = AF_INET;
    inet_pton(AF_INET, chHostAddr, &hostAddr.sin_addr);

    //�����շ���ʱʱ��
    int timeout = DEFAULT_RECV_TIMEOUT_USEC;
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    if (ret == -1) {
        std::cout << "set socket option error" << std::endl;
        closesocket(sock);
        return -1;
    }

    //ѭ�����͡����ܱ��ģ�����������Ϣ
    int count = 4;//��Ҫ���ͱ�������
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

//��װ����

PingTest::PingTest(const std::string hostAddr)
    :m_host(hostAddr),
    m_count(DEFAULT_SEND_COUNT),
    m_infinite(DEFAULT_INFINITE),
    m_timeout(DEFAULT_RECV_TIMEOUT_USEC)
{
}

int PingTest::Ping()
{
    //����socket
    SOCKET sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (INVALID_SOCKET == sock)
    {
        std::cout << "socket error: " << WSAGetLastError() << std::endl;
        return -1;
    }

    //ip��ַתΪ�ṹ��
    sockaddr_in hostAddr = {
        .sin_family = AF_INET,//�ṹ���ʼ����һ��д��
        .sin_port = 0,
        .sin_addr = {0},
        .sin_zero = {0}
    };
    inet_pton(AF_INET, m_host.c_str(), &hostAddr.sin_addr);

    //�����շ���ʱʱ��
    int ret = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&m_timeout, sizeof(m_timeout));
    if (ret == -1) {
        std::cout << "set socket option error" << std::endl;
        closesocket(sock);
        return -1;
    }

    //���͡�����
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

    //send�����ܰ����ݲ��ֺͱ���ͷ�ֿ����ͣ������൱�ڷ�������icmp���ģ����ݲ��ֵı��Ļ���Ϊicmp��ʽ����ȷ���޷����������ȷʶ��Ӧ�ò�ı��Ļ������Էֿ�����
    int bytes = sendto(sock, (const char*)&icmp, sizeof(icmp), 0, (sockaddr*)addr, sizeof(*addr));
    m_sendTime = std::chrono::system_clock::now();//��¼����ʱ��
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

    //����IP����
    int bytes = recvfrom(sock, buffer, sizeof(buffer), 0, nullptr, nullptr);
    m_recvTime = std::chrono::system_clock::now();//��¼����ʱ��
    if (SOCKET_ERROR == bytes) {
        int err = WSAGetLastError();
        std::cout << "recvfrom failed, error: " << err << std::endl;
        return -1;
    }

    //��¼����ʱ�䣬ת��Ϊ΢�룬��΢��Ϊ��λ
    auto usedTime = m_recvTime - m_sendTime;
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(usedTime);

    //��ȡicmp����
    int ip_header_len = (buffer[0] & 0xf) << 2;//���ĳ��ȱ���Ϊ4��������������ͷ�м�¼���������
    icmp = (ICMP_ECHO_REQ*)(buffer + ip_header_len);

    //��ȡicmpͷ����Ϣ
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
        addr += 2;//��Ϊ��short������ԭ��char��ָ����Ҫ�ƶ�����
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