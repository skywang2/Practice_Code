1. 使用libevent监听一个管道文件，pipe_read
2. 写一个向管道写入数据的程序，pipe_write
3. 写一个监听信号的程序，signal_read
4. 写一个高并发服务程序，listener
5. makefile只编译了pipe_read.cpp，其他几个可以加载makefile的TARGET中或者手动gcc编译，需要加上-std=c++11、-levent
