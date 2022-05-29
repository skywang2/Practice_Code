1. 静态库名helloworld，静态库文件名libhelloworld.a
2. 静态库包含Util类
3. 如何创建静态库
    - 生成.o中间文件：g++ -c helloworld.cc
    - 生成静态库：ar -crv libhelloworld.a helloworld.o
    - ar是一个打包命令，-c创建库，-r将新增条目加入库
    - 静态库命名规则：lib[your_library_name].a
4. 如何调用静态库
    - 在调用静态库的文件中添加库对应的头文件
    - g++ -o 输出文件名 xxx.cc -L库路径(-L后无空格) -l库名