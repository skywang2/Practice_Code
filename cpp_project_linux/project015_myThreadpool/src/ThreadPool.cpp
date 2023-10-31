#include "ThreadPool.h"
#include <iostream>
#include <cstring>

#define SAFE_DELETE(x) do{ if(x) { delete x; x = nullptr; } }while(0)
#define SAFE_FREE(x) do{ if(x) { free(x); x = nullptr; } }while(0)

/// @brief 初始化线程池
/// @param threadCount 
/// @param maxJob 
MyThreadPool::MyThreadPool(int threadCount, int maxJob)
    :m_maxJob(maxJob),
    m_threadCount(threadCount),
    m_freeThreadCount(threadCount)
{
    //入参检查
    if(m_threadCount < 1 || m_maxJob < 1)
    {
        std::cerr << "threadCount or maxJob error" << std::endl;
        return;
    }
    //初始化worker
    m_workers = new WORKER[m_threadCount];
    if(!m_workers)
    {
        std::cerr << "init workers error" << std::endl;
        return;
    }
    for(int i = 0; i < m_threadCount; i++)
    {
        //先构造worker再启动线程，最后修改worker中的值，避免thread执行太快worker为空
        m_workers[i] = 
        {
            .threadid = std::thread::id(0),
            .terminate = false,
            .isWorking = false,
            .pool = this,
        };
        std::thread th(_Run, static_cast<void*>(&m_workers[i]));// 启动线程，传入线程控制块
        m_workers[i].threadid = th.get_id();

        th.detach();
    }
}

/// @brief 结束所有线程
MyThreadPool::~MyThreadPool()
{
    //设置结束标记
    for(int i = 0; i < m_threadCount; i++)
    {
        m_workers[i].terminate = true;
    }
    //唤醒所有线程
    {
        std::unique_lock<std::mutex> lck(m_jobMutex);
        m_jobCond.notify_all();
    }

    delete[] m_workers;
}

/// @brief job加入队列，用户接口
/// @param func 
/// @param args 
/// @param argsLen 
/// @return 
int MyThreadPool::pushJob(JobFunc func, void *args, int argsLen)
{
    JOB* job = new JOB;
    if(!job)
    {
        std::cerr << "create job error" << std::endl;
        return -1;
    }
    job->func = func;
    job->user_data = calloc(argsLen, 1);
    memcpy(job->user_data, args, argsLen);

    _AddJob(job);

    return 0;
}

/// @brief job加入队列
/// @param job 
/// @return 
bool MyThreadPool::_AddJob(JOB* job)
{
    std::unique_lock<std::mutex> lck(m_jobMutex);
    if(m_jobList.size() >= m_maxJob)
    {
        std::cerr << "add job error, job list is full" << std::endl;
        return false;
    }
    m_jobList.push_back(job);
    m_jobCond.notify_one();//注意唤醒前需要锁定wait时用的锁

    return true;
}

/// @brief 线程核心函数
/// @param args 
/// @return 
void *MyThreadPool::_Run(void *args)
{
    WORKER* worker = static_cast<WORKER*>(args);
    worker->pool->_ThreadRun(args);
    return nullptr;
}

/// @brief 线程核心函数，调用job保存的回调函数
/// @param args 
void MyThreadPool::_ThreadRun(void *args)
{
    WORKER* worker = static_cast<WORKER*>(args);

    while(true)
    {
        JOB* job = nullptr;

        //从job队列取任务，需要加锁
        {
            std::unique_lock<std::mutex> lck(m_jobMutex);
            //没有任务，等待有任务和被唤醒
            m_jobCond.wait(lck, [&](){ return worker->terminate || !m_jobList.empty(); });// 先进行第二个参数的返回值判断，再wait，第二个参数返回值为true则可以往下执行，为false则继续wait
            if(worker->terminate && m_jobList.empty()) { break; }// 执行完队列中所有任务再结束

            //有任务，取一个出来
            job = m_jobList.front();
            m_jobList.pop_front();
        }

        //执行任务
        m_freeThreadCount--;
        worker->isWorking = true;
        job->func(job->user_data);//core
        worker->isWorking = false;
        m_freeThreadCount++;

        //释放该任务相关资源
        SAFE_FREE(job->user_data);
        delete job;
    }
}
