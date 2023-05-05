#ifndef ICMP_HEAD_H
#define ICMP_HEAD_H

#include <Windows.h>

struct icmp   //占8字节，在<netinet/ip_icmp.h>头文件中定义
{
	UINT8 type; //类型
	UINT8 code; //代码
	UINT16 checksum; //检验和
	UINT16 id;  //标识符
	UINT16 sequence; //序列号
	//UINT32 send_time;//发起时间戳
	//UINT32 recv_time;//接收时间戳
	//UINT32 route_time;//传送时间戳
};

#endif
