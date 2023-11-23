#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <memory>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <event.h>

int g_signalCount = 0;

/// @brief 回调函数，读取fifo
/// @param fd 
/// @param events 
/// @param arg 
void signal_handler(evutil_socket_t fd, short events, void* arg) {
  event* ev = static_cast<event*>(arg);
  if (!ev) {
    perror("arg is null");
    return;
  }
  
  g_signalCount++;
  if (g_signalCount >= 3)
  {
    event_del(ev);//从事件集合移除
  }
  
  printf("get signal\n");
}

int main() {
  //初始化
  event_base* base = event_base_new();
  if (!base) {
    perror("base new error");
    return 1;
  }

  {
    //创建事件并绑定fd
    event ev;
    event_assign(&ev, base, SIGINT, EV_SIGNAL | EV_PERSIST, signal_handler, &ev);
    
    //将事件添加到集合
    event_add(&ev, nullptr);

    //事件循环
    event_base_dispatch(base);

    event_del(&ev);
  }

  event_base_free(base);
  return 0;
}
