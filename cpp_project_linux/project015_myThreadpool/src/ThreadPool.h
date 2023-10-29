#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <thread>
#include <mutex>
#include <condition_variable>
#include <list>
#include <atomic>
#include <memory>

class MyThreadPool 
{
private:
    using JobFunc = void (*)(void* arg);
    struct WORKER
    {
        std::thread::id threadid;
        bool terminate;
        int isWorking;
        MyThreadPool* pool;//线程归属哪个线程池
    };

    struct JOB
    {
        JobFunc func;
        void* user_data;
    };
        
public:
    MyThreadPool(int threadCount, int maxJob);// 初始化线程池
    ~MyThreadPool();
    int pushJob(JobFunc func, void* args, int argsLen);//添加任务

private:
    bool _AddJob(JOB* job);// 内部添加任务
    static void* _Run(void* args);// 线程函数，传入this，通过this执行线程池函数的基本功能
    void _ThreadRun(void* args);//被Run调用，从队列里取任务并执行

    int m_maxJob;
    int m_threadCount;
    std::atomic<int> m_freeThreadCount;
    // std::atomic<bool> m_initOver;
    WORKER* m_workers;
    std::list<JOB*> m_jobList;
    std::mutex m_jobMutex;
    std::condition_variable m_jobCond;
};

#endif//THREADPOOL_H
