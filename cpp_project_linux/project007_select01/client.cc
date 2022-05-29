#include <stdio.h>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

int main(int argc, char* argv[])
{
    int errorCode;

    //address
    sockaddr_in clientAddr;
    memset(&clientAddr, 0, sizeof(clientAddr));
    clientAddr.sin_family = AF_INET;
    clientAddr.sin_port = htons(3333);
    clientAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    //socket
    int clientSockfd = -1;
    clientSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(-1 == clientSockfd)
    {
        cout << "client socket create failed." << endl;
	    return 1;
    }
    cout << "clientfd:" << clientSockfd << endl;

    //connect 
    errorCode = connect(clientSockfd, (sockaddr*)&clientAddr, sizeof(clientAddr));
    if(errorCode < 0)
    {
        cout << "client can't connect to server." << endl;
        return 1;
    }
    cout << "client connect successfully." << endl;    

    char text[1024];
    while(1)
    {
        memset(text, 0, 1024);
        cin >> text;
        //sprintf(text, "client pid:%d, content:", getpid(), text);
        write(clientSockfd, text, sizeof(text));
        
        memset(text, 0, 1024);
        read(clientSockfd, text, sizeof(text));
        cout << text << endl;
    }

    close(clientSockfd);
    return 0;
}
