#pragma once

#include <mutex>
#include <memory>
#include <iostream>


void Deleter(std::mutex* mtx) { mtx->unlock(); }

class MyLockerUncopy
{
	//function object
	class MyLockerDeleter
	{
	public:
		void operator() (std::mutex* mtx) { mtx->unlock(); }
	};

public:
	explicit MyLockerUncopy(std::mutex* mtx)
		:m_pMtx(mtx, m_deleter)//指定删除器
	{
		m_pMtx.get()->lock();
		std::cout << "locked" << std::endl;
	}

	~MyLockerUncopy()
	{
		//m_pMtx.get()->unlock();//m_pMtx初始化时已经赋予了删除器，无需自己手动调用
		std::cout << "unlocked" << std::endl;
	}

private:
	MyLockerUncopy(MyLockerUncopy&) = delete;
	MyLockerUncopy& operator= (const MyLockerUncopy&) = delete;

	MyLockerDeleter m_deleter;//定义一个删除器，可以用全局函数或者函数对象
	std::shared_ptr<mutex> m_pMtx;

};
