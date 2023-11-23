#include <stdio.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <event.h>
#include <event2/listener.h>

#define BUFFSIZE 10

//ctx对应bufferevent_setcb最后一个参数
void read_cb(struct bufferevent* bev, void* ctx) {
    char buff[BUFFSIZE] = { 0 };
    evbuffer* evbuff = bufferevent_get_input(bev);
    size_t ret = 0;

    while (evbuffer_get_length(evbuff) > 0)
    {
        memset(buff, 0, sizeof(buff));
        ret = bufferevent_read(bev, buff, sizeof(buff) - 1);
        if (ret < 0) {
            printf("bufferevent_read eror\n");
            break;
        }
        else {
            printf("%s", buff);
        }
    }
    printf("\n");
}

void write_cb(struct bufferevent* bev, void* ctx) {
    
}

void event_cb(struct bufferevent* bev, short what, void* ctx) {
    if (what & BEV_EVENT_EOF) {
        printf("eof file reached\n");
    }
    if (what & BEV_EVENT_READING) {
        printf("error encountered while reading\n");
    }
    if (what & BEV_EVENT_WRITING) {
        printf("error encountered while writing\n");
    }
    if (what & BEV_EVENT_ERROR) {
        printf("unrecoverable error encountered\n");
    }
    if (what & BEV_EVENT_TIMEOUT) {
        printf("user-specified timeout reached\n");
    }
    if (what & BEV_EVENT_CONNECTED) {
        printf("connect operation finished\n");
    }

    bufferevent_free(bev);
}

void listener_cb(evconnlistener* listener, evutil_socket_t fd, sockaddr* addr, int socklen, void* arg) {
    printf("connection get\n");
    event_base* base = static_cast<event_base*>(arg);

    //缓冲对象，和fd绑定
    int option = BEV_OPT_CLOSE_ON_FREE;
    bufferevent* buff = bufferevent_socket_new(base, fd, option);
    if (!buff) {
        printf("bufferevent_socket_new error\n");
        return;
    }
    bufferevent_setcb(buff, read_cb, write_cb, event_cb, nullptr);

    //使能某种事件类型
    bufferevent_enable(buff, EV_READ | EV_WRITE);
}

int main() {
    sockaddr_in sa;
    memset(&sa, 0, sizeof(sa));
    sa.sin_family = AF_INET;
    sa.sin_port = htons(4444);
    inet_aton("127.0.0.1", &sa.sin_addr);

    event_base* base = event_base_new();
    if (!base) {
        printf("event_base_new error\n");
        return 1;
    }

    //创建监听对象，自动完成bind、listen、accept
    int flags = LEV_OPT_CLOSE_ON_FREE | LEV_OPT_CLOSE_ON_EXEC | LEV_OPT_REUSEABLE;
    evconnlistener* listener = evconnlistener_new_bind(base, listener_cb, base,
        flags, 1000, (sockaddr*)&sa, sizeof(sa));
    if (!listener) {
        printf("evconnlistener_new_bind error\n");
        return 1;
    }

    event_base_dispatch(base);

    evconnlistener_free(listener);

    event_base_free(base);
    return 0;
}
