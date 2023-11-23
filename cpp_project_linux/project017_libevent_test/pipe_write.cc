#include <string.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("fifo.tmp", O_WRONLY);
    if (-1 == fd) {
        perror("open error");
        return 1;
    }

    
    int s = 1024, r = 32;
    //生成格式化字符串模板，限制%s的读取字符个数
    const int rdsize = r;
    char sRdBuff[rdsize];
    memset(sRdBuff, 0, rdsize);
    snprintf(sRdBuff, rdsize, "%%%ds", s - 1);    
    
    const int size = s;
    char buff[size];
    while (1) {        
        memset(buff, 0, size);
        scanf(sRdBuff, buff);
        
        int ret = write(fd, buff, strlen(buff));
        if (ret <= 0) {
            break;
        }
    }

    close(fd);
    return 0;
}