#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>

#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/*fn Writen
 *brief write 'count' bytes to fd
 *param_in fd, handle
 *param_in buf, buffer to write
 *param_in count, write 'count' bytes
 *param_in time, timeout
 *return
 * */
int Writen(int fd, const char* buf, int count, int time=0)
{
    int wLeft = count;
    const char* cur = buf;
    //select time--------------------------
    if(fd < 0) return false;
    
    while(wLeft > 0)
    {
        int n;
        n = write(fd, cur, count);
        wLeft -= n;
        cur += n;
    }

    return (count - wLeft);
}

/*fn Readn
 *brief read 'count' bytes from fd
 *param_in fd, handle
 *param_out buf, save buffer
 *param_in count, read 'count' bytes
 *param_in time, timeout
 *return the count had read
 * */
int Readn(int fd, char* buf, int count, int time=0) {
    int rLeft = count;
    char* cur = buf;
    if(fd < 0) return false;

    while(rLeft > 0)
    {
        int n;
        n = read(fd, cur, rLeft);
        rLeft -= n;
        cur += n;
    }
    return (count - rLeft); 
}

bool initServer(int &sockFd)
{
    int errorCode;

    //address
    sockaddr_in servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(3333);

    //socket
    int servSockfd;
    servSockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(-1 == servSockfd)
    {
        cout << "socket creat failed, error:" << servSockfd << endl;
        return false;
    }
    int opt = 1;
    setsockopt(servSockfd, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));
    //bind
    errorCode = bind(servSockfd, reinterpret_cast<sockaddr*>(&servAddr), sizeof(servAddr));
    if(-1 == errorCode)
    {
        cout << "bind failed, error:" << errorCode << endl;
        return false;
    }
    
    //listen
    errorCode = listen(servSockfd, 10);
    if(-1 == errorCode)
    {
        cout << "listen failed, error:" << errorCode << endl;
        return false;
    }

    sockFd = servSockfd;
    return true;
}

inline bool UpdateMaxfd(int &maxfd, int &fd, fd_set* readfds)
{
    if(maxfd < 0 || fd < 0) return false;

    if(fd == maxfd)
    {
        for(int i = maxfd; i > 0; i--)
        {
            if(FD_ISSET(i, readfds))
            {//原本最大的已经去除了
                maxfd = i;
                break;
            }
        }
    }
    return true;
}

//#define TEST_WR
//#define TEST_SELECT_CIN
#define TEST_SELECT_SOCKET

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for(char** p = argv; p != argv + argc; ++p) {
	args.push_back(*p);
    }
    for(std::vector<std::string>::iterator i = args.begin(); i != args.end(); ++i) {
	std::cout << *i << std::endl;
    }

#ifdef TEST_WR
    int fileFd = open("test.txt", O_RDWR | O_CREAT);
    int w = Writen(fileFd, "123456", 3);
    std::cout << "w:" << w << std::endl;
    char buf[32];
    memset(buf, 0, 32);
    lseek(fileFd,0,SEEK_SET);
    int r = Readn(fileFd, buf, 8);
    std::cout << "r:" << r << std::endl;
    std::cout << buf << std::endl;
    close(fileFd);
    return 0;
#endif

#ifdef TEST_SELECT_CIN
    fd_set rfds;
    struct timeval tv;
    int ret = -1;
    memset(&tv, 0, sizeof(tv));

    while(true) {
	FD_ZERO(&rfds);
	FD_SET(STDIN_FILENO, &rfds);
    	tv.tv_sec = 5;
    	tv.tv_usec = 0;
	ret = select(1, &rfds, NULL, NULL, &tv);
	if(-1 == ret) {
	    std::cout << "ret:" << ret << ", select error." << std::endl;
	    break;
	} else if(0 == ret) {
	    std::cout << "ret:" << ret << ", timeout." << std::endl;
	} else if(ret) {
	    std::cout << "ret:" << ret << "." << std::endl;
	    if(FD_ISSET(STDIN_FILENO, &rfds)) {
		char temp[64];
		memset(temp, 0, 64);
		std::cout << "select ok." << std::endl;
		std::cin >> temp;
		std::cout << temp << std::endl;
	    }
	}
    }
    return 0;
#endif

#ifdef TEST_SELECT_SOCKET
    int serverfd = -1;
    int maxfd = 0;
    int ret = -1;
    fd_set readfds, tmpfds;
    struct timeval tv;
    ::memset(&tv, 0, sizeof(tv));

    if(!initServer(serverfd)) return 1;
    FD_ZERO(&readfds);
    FD_SET(serverfd, &readfds);
    maxfd = serverfd;
    std::cout << "serverfd:" << serverfd << std::endl;

    while(1) {
        fd_set tmpfds = readfds;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        ret = select(maxfd + 1, &tmpfds, NULL, NULL, &tv);
        if(-1 == ret)
        {
            std::cout << "ret:" << ret << ", select error." << std::endl;
            break;
        }
        if(0 == ret)
        {
            std::cout << "ret:" << ret << ", timeout." << std::endl;
            continue;
        }

        for(int fd = 0; fd <= maxfd; fd++)
        {
            if(FD_ISSET(fd, &tmpfds) <= 0)
            {
                //std::cout << "fd is not in tmpfds." << std::endl;
                continue;
            }

            if(fd == serverfd)
            {
                int clientfd = accept(serverfd, NULL, NULL);
                if(clientfd < 0)
                {
                    std::cout << "accept() failed." << std::endl;
                    continue;
                }
                std::cout << "clientfd:" << clientfd << " is connected." << std::endl;
                FD_SET(clientfd, &readfds);
                if(maxfd < clientfd) maxfd = clientfd;
                continue;
            }
            else
            {
                char buff[1024];
                ::memset(buff, 0, sizeof(buff));
                size_t nums = read(fd, buff, sizeof(buff));
                if(nums <= 0)
                {//disconnect, -1 is fail, 0 is shutdown by peer
                    std::cout << "clientfd:" << fd << " disconnect, read nums <= 0." << std::endl;
                    close(fd);
                    FD_CLR(fd, &readfds);
                    UpdateMaxfd(maxfd, fd, &readfds);
                    continue;
                }
                std::cout << "clientfd:" << fd << " read:" << buff << std::endl;
                ::sprintf(buff, "hello, i'm server.");
                write(fd, buff, sizeof(buff));
            }
        }//for   
    }//while(1)
#endif

    return 0;
}


