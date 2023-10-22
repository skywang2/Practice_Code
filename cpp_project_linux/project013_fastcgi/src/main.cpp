#include <cstdlib>

#include <unistd.h>

#include <fcgi_stdio.h>
//#include <fcgi_config.h> // 可选，看是否要读取配置文件

int main(int argc, char* argv[])
{
    // FCGI_Accept()是一个阻塞函数, nginx给fastcgi程序发送数据的时候解除阻塞
    while (FCGI_Accept() >= 0) 
    {
        // 1. 接收数据
        // 1.1 get方式提交数据 - 数据在请求行的第二部分
        // user=zhang3&passwd=123456&age=12&sex=man
        char *query = getenv("QUERY_STRING"); 
        // 1.2 post方式提交数据
        // char *contentLength = getenv("CONTENT_LENGTH");
        // 根据长度大小判断是否需要循环
        // 2. 按照业务流程进行处理
        // 3. 将处理结果发送给nginx
        // 数据回发的时候, 需要告诉nginx处理结果的格式 - 假设是html格式
        printf(
        "Content-type: text/html\r\n"
        "\r\n"
        "<html>"
        "<p>QUERY_STRING:%s</p>"
        "<p>2</p>"
        "<p>3</p>"
        "<p>4</p>"
        "</html>",
        query);
    }

    return 0;
}
