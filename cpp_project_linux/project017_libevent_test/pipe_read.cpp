#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <event.h>

/// @brief 回调函数，读取fifo
/// @param fd 
/// @param events 
/// @param arg 
void fifo_read(evutil_socket_t fd, short events, void * arg) {
  int s = 32;
  const int size = s;
  char buff[size];
  memset(buff, 0, size);

  int ret = read(fd, buff, size - 1);
  printf("fifo read: %s\n", buff);
}

int main() {
  if (access("fifo.tmp", F_OK) == -1) {
    int ret = mkfifo("fifo.tmp", S_IRWXU);
    if(-1 == ret) {
      perror("mkfifo error");
      return 1;
    }
  }

  int fd = open("fifo.tmp", O_RDONLY);
  if(-1 == fd) {
    perror("open error");
    return 1;
  }

  //初始化
  event_base* base = event_base_new();
  if (!base) {
    perror("base new error");
    return 1;
  }

  //创建事件并绑定fd
  event* ev = event_new(base, fd, EV_READ | EV_PERSIST, fifo_read, nullptr);
  
  //将事件添加到集合
  event_add(ev, nullptr);

  //事件循环
  event_base_dispatch(base);

  event_free(ev);
  event_base_free(base);
  return 0;
}

