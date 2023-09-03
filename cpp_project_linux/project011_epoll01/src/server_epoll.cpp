#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
// #include <sys/poll.h>
#include <unistd.h>
#include <fcntl.h>
#include <memory.h>
#include <memory>
#include <iostream>

const short port = 3333;
const int maxEventCount = 1024;

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

int main(int argc, char* argv[])
{
    std::cout << "program start" << std::endl;

    //创建监听socket
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listenfd)
    {
        std::cout << "create listen socket error" << std::endl;
        return -1;
    }
    //设置ip、端口重用
    int on = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, (void*)&on, sizeof(on));
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEPORT, (void*)&on, sizeof(on));
    //将监听socket设置为非阻塞
    /*当内核缓冲区满时，阻塞模式socket的send、recv函数会阻塞，而非阻塞模式不会*/
    int oldSocketFlag = fcntl(listenfd, F_GETFL, 0);
    int newSocketFlag = oldSocketFlag | O_NONBLOCK;
    if(-1 == fcntl(listenfd, F_SETFL, newSocketFlag))
    {
        close(listenfd);
        std::cout << "set listenfd to nonblock error" << std::endl;
        return -1;
    }
    //初始化服务器ip
    sockaddr_in bindaddr;
    bindaddr.sin_family = AF_INET;
    bindaddr.sin_addr.s_addr = htonl(INADDR_ANY);//监听本机所有网卡的ip
    bindaddr.sin_port = htons(port);
    //socket绑定ip、port
    if(-1 == bind(listenfd, (sockaddr*)&bindaddr, sizeof(bindaddr)))
    {
        close(listenfd);
        std::cout << "bind listenfd error" << std::endl;
        return -1;
    }
    //开始监听
    if(-1 == listen(listenfd, SOMAXCONN))
    {
        close(listenfd);
        std::cout << "listen error" << std::endl;
        return -1;
    }

    //创建epollfd
    int epollfd = epoll_create(1);
    if(-1 == epollfd)
    {
        close(listenfd);
        std::cout << "create epollfd error" << std::endl;
        return -1;
    }
    //创建epoll event，绑定监听socket到epoll
    epoll_event listen_event;
    listen_event.data.fd = listenfd;
    listen_event.events = EPOLLIN;
    if(-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, listenfd, &listen_event))
    {
        close(listenfd);
        std::cout << "epoll_ctl error" << std::endl;
        return -1;
    }

    while (true)
    {
        //准备接收事件，连接事件、数据收发事件（IN、OUT）
        epoll_event epollEvents[maxEventCount];
        memset(epollEvents, 0, sizeof(epoll_event) * maxEventCount);
        int result = epoll_wait(epollfd, epollEvents, maxEventCount, 1000);
        if(result < 0)
        {
            if(EINTR == errno)
            {
                continue;
            }
            break;
        }
        else if(0 == result)
        {
            std::cout << "." << std::flush;            
            continue;
        }
        std::cout << std::endl;

        //遍历epollfd收到的所有事件
        for(int i = 0; i < result; ++i)
        {
            //处理读事件
            if(epollEvents[i].events & EPOLLIN)
            {
                //处理监听socket与已连接socket
                if(epollEvents[i].data.fd == listenfd)
                {
                    //接受新连接
                    sockaddr_in clientaddr;
                    socklen_t clientaddrLen = sizeof(clientaddr);
                    int clientfd = accept(epollEvents[i].data.fd, (sockaddr*)&clientaddr, &clientaddrLen);
                    if(-1 != clientfd)
                    {
                        //设置clientfd的属性
                        int oldSocketFlag = fcntl(clientfd, F_GETFL, 0);
                        int newSocketFlag = oldSocketFlag | O_NONBLOCK;
                        if(-1 == fcntl(clientfd, F_SETFL, newSocketFlag))
                        {
                            close(clientfd);
                            std::cout << "set clientfd to nonblock error" << std::endl;
                            continue;                         
                        }
                        //将clientfd添加到epollfd事件中
                        epoll_event client_event;
                        client_event.data.fd = clientfd;
                        client_event.events = EPOLLIN;
                        // listen_event.events |= EPOLLET;
                        if(-1 == epoll_ctl(epollfd, EPOLL_CTL_ADD, clientfd, &client_event))
                        {
                            close(clientfd);
                            std::cout << "add new clientfd error" << std::endl;
                        }
                        else
                        {
                            std::cout << "add new clientfd:" << clientfd << std::endl;
                        }
                    }
                    else
                    {
                        std::cout << "accept clientfd error" << std::endl;
                    }
                }
                else
                {
                    // sleep(2);
                    std::cout << "client fd:" << epollEvents[i].data.fd << " receive data" << std::endl;
                    char ch;//LT模式下，每次读取1个字符
                    int recvNum = recv(epollEvents[i].data.fd, &ch, 1, 0);
                    std::cout << "recvnum:" << recvNum << std::endl;
                    if(0 == recvNum)
                    {
                        //读完了，对端关闭连接，从epollfd上移除clientfd
                        if(-1 != epoll_ctl(epollfd, EPOLL_CTL_DEL, epollEvents[i].data.fd, nullptr))
                        {
                            std::cout << "receive 0 from client, disconnect" << std::endl;
                        }
                        close(epollEvents[i].data.fd);//不论是否从epollfd中移除，都必须关闭
                    }
                    else if(recvNum < 0)
                    {
                        //读取出错，EWOULDBLOCK、EINTR情况下需要重新读取，故不处理
                        if(EWOULDBLOCK != recvNum && EINTR != recvNum)
                        {
                            if(-1 != epoll_ctl(epollfd, EPOLL_CTL_DEL, epollEvents[i].data.fd, nullptr))
                            {
                                std::cout << "receive error, disconnect" << std::endl;
                            }
                            close(epollEvents[i].data.fd);//不论是否从epollfd中移除，都必须关闭
                        }
                    }
                    else
                    {
                        //正常读取
                        std::cout << "receive:" << ch << std::endl;
                    }
                }
            }
            else if(epollEvents[i].events & EPOLLOUT)
            {
                //do nothing
            }
            else if(epollEvents[i].events & EPOLLERR)
            {
                //todo:do something
            }
            else
            {
                //do nothing
            }
        }
    }    

    close(listenfd);
    return 0;
}