#include <stdio.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <utility>
#include <unistd.h>
#include <sys/socket.h>//å£°æç½ç»ç¼ç¨æå¸¸ç¨çå½æ°
#include <arpa/inet.h>//ipåportæ°æ®ç»æçå®ä¹ä¸è½¬æ¢å½æ°
#include <signal.h>

using std::cout;
using std::endl;
using std::pair;
using std::vector;

const int maxPreLinkSock = 10;//æªå®æè¿æ¥å¥æ¥å­æ»æ°
//int clientSockFds[maxPreLinkSock];
vector<std::pair<int, sockaddr_in> > sockAddrs;

int main(int argc, char* argv[]) {
    int errorCode;
    
    //address
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(3333);//è®¾å®3333ä¸ºæå¡å¨çå¬ç«¯å£
    
    //socket
    int servSockfd;
    servSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(-1 == servSockfd) {
        cout << "socket creat failed, error:" << servSockfd << endl;
        return 1;
    }
    
    //bind
    errorCode = bind(servSockfd, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr));
    if(-1 == errorCode) {
        cout << "bind failed, error:" << errorCode << endl;
        return 1;
    }
    
    //listen
    errorCode = listen(servSockfd, maxPreLinkSock);
    if(-1 == errorCode) {
        cout << "listen failed, error:" << errorCode << endl;
        return 1;
    }
    
    //accept
    socklen_t len = sizeof(sockaddr_in);    
    signal(SIGCHLD, SIG_IGN);
    while(true) {
        sockaddr_in cAddr;
        memset(&cAddr, 0, sizeof(cAddr));
        int clientSocket = accept(servSockfd, reinterpret_cast<sockaddr*>(&cAddr), &len);
        if(-1 == clientSocket) {
            cout << "accept failed, error:" << clientSocket << endl;
            continue;
        }
	if(0 == fork()) {
	    char text[32];
	    memset(text, 0, 32);
	    read(clientSocket, text, 32);
	    cout << text << endl;
	    
	    memset(text, 0, 32);
	    sprintf(text, "hello..."); 
	    write(clientSocket, text, sizeof(text));
	    break;
	}
    }

    //
    return 0;
}
