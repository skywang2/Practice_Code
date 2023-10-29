#include "ThreadPool.h"
#include <mutex>
#include <iostream>
#include <chrono>

std::mutex g_mtx;

void TestFunc(void* arg)
{
    static int t = 0;

    std::unique_lock<std::mutex> mtx(g_mtx);
    //std::cout << "t=" << t++ << std::endl;
}

int main()
{
    MyThreadPool* pool = new MyThreadPool(800, 10000);
    std::cout << "=====start=====" << std::endl;
    for(int i = 0; i < 10000; i++)
    {
        pool->pushJob(TestFunc, &i, sizeof(i));
    }

    //std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}
