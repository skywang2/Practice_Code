#include <cstdio>
#include <iostream>

int main() {
    int ret = 0;

    rename("test.txt", "test_rename.txt");//这是注释
    if(ret<0)
    {
     std::cout << "rename error\n" << std::endl;
    }

    return 0;
}