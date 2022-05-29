1. 动态库名log，动态库文件名liblog.so
2. 动态库包含MyLoger类
3. 如何创建动态库
    - 创建.o中间文件：g++ -fPIC -c log.cc
    - 生成动态库，g++ -shared -o liblog.so log.o
    - 上述两步可合成一步：g++ -shared -fPIC -o liblog.so log.cc
4. 如何调用动态库
    - 在调用动态库的文件中添加库对应的头文件
    - g++ -o 输出文件名 xxx.cc -L库路径(-L后无空格) -l库名
    - 将生成的.so文件复制到/lib或/usr/lib或/usr/lib64，或者使用export LD_LIBRARY_PATH="lib路径":$LD_LIBRARY_PATH，其中export的方法是临时的，关闭控制台后失效