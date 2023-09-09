#include <unistd.h>
#include <mqueue.h>
#include <signal.h>
#include <fcntl.h>
#include <iostream>

/*
getopt会逐个检查选项，并把每个选项的参数存在optarg中，每次循环会覆盖
遇到没在optstring中定义的选项，会返回'?'，可以进行特殊处理，比如输出help信息

optstring扫描规则：
1.单字符，表示不带参数的选项；
2.单字符+1个冒号，表示选项必须带参数字符串，选项和参数之间有空格
3.单字符+2个冒号，表示选项的参数可选，选项和参数之间无空格

自定义错误处理：
使用条件：（满足1条即可）
1.在调用getopt()之前，将opterr设置为0，这样就可以在getopt()函数发现错误的时候强制它不输出任何消息，但是还是会返回一个‘?’
2.如果optstring参数的第一个字符是‘:’，那么getopt()函数没有默认提示，并根据错误情况返回不同字符:
    - “无效选项” —— getopt()返回’?’，并且optopt包含了无效选项字符。
    - “缺少选项参数” —— getopt()返回 ‘:’ ，optopt包含了无效字符
*/
void DealArgv(int argc, char *const argv[])
{
    if (argc < 1)
    {
        return;
    }

    int opt = -1;
    while (-1 != (opt = getopt(argc, argv, ":ab:c:de::")))
    {
        // std::cout << "optind:" << optind << std::endl;
        switch (opt)
        {
        case 'a':
            std::cout << "-" << (char)opt << std::endl;
            break;
        case 'b':
            std::cout << "-" << (char)opt << std::endl;
            std::cout << "optarg:" << optarg << std::endl;
            break;
        case 'c':
            std::cout << "-" << (char)opt << std::endl;
            std::cout << "optarg:" << optarg << std::endl;
            break;
        case 'd':
            std::cout << "-" << (char)opt << std::endl;
            break;
        case 'e':
            std::cout << "-" << (char)opt << std::endl;
            std::cout << "optarg:" << optarg << std::endl;
            break;
        case '?'://无效选项
            std::cout << "Invalid Option:" << (char)optopt << std::endl;
            break;
        case ':'://缺少选项参数
            std::cout << "Missing Parameter:" << (char)optopt << std::endl;
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    DealArgv(argc, argv);

    int flags = O_RDWR | O_CREAT;

    return 0;
}