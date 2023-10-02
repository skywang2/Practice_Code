#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <cstdio>
#include <iostream>
#include <string>

#include "MyPing.h"
#include "GETOPT.h"

#pragma comment(lib, "ws2_32.lib")

// ctrl+/提供自动注释

/// <summary>
/// this is summary.
/// </summary>
/// <param name="argc">传入参数个数</param>
/// <param name="argv">传入参数值</param>
/// <returns>int</returns>
int main(int argc, char* argv[])
{
    int iResult = 0;
    DWORD dwError;
    WSADATA wsaData;
    hostent* remoteHostInfo = nullptr;
    struct in_addr remoteAddr;
    memset(&remoteAddr, 0, sizeof(remoteAddr));

    //start
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return -1;
    }
    
    //parse parameters
//#define IN_VS
#ifdef IN_VS
    int option = 0;
    std::string optstring(":a:");
    while (-1 != (option = getopt(argc, argv, optstring.data())))
    {
        switch (option)
        {
        case 'a'://域名或ip
            remoteHostInfo = gethostbyname(optarg);
            break;
        case '?':
            std::cout << "非法选项" << optopt << std::endl;
            break;
        case ':':
            std::cout << "选项" << optopt << "缺少参数" << std::endl;
            break;
        default:
            std::cout << "非法解析" << std::endl;
        }
    }
#else
    remoteHostInfo = gethostbyname("baidu.com");
#endif

    if (remoteHostInfo == NULL) {
        dwError = WSAGetLastError();
        if (dwError != 0) {
            if (dwError == WSAHOST_NOT_FOUND) {
                printf("Host not found\n");
                return -1;
            }
            else if (dwError == WSANO_DATA) {
                printf("No data record found\n");
                return -1;
            }
            else {
                printf("Function failed with error: %ld\n", dwError);
                return -1;
            }
        }
    }
    else
    {
        if (remoteHostInfo->h_addrtype == AF_INET) {
            int i = 0;
            while (remoteHostInfo->h_addr_list[i] != 0) {
                remoteAddr.s_addr = *(u_long*)remoteHostInfo->h_addr_list[i++];
                printf("IPv4 Address #%d: %s\n", i, inet_ntoa(remoteAddr));
            }
        }
        else if (remoteHostInfo->h_addrtype == AF_INET6)
        {
            printf("Remotehost is an IPv6 address\n");
        }
    }

    //ping
    Ping(inet_ntoa(remoteAddr));

    //end
    WSACleanup();
    system("pause");
	return 0;
}