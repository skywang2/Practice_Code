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
		:m_pMtx(mtx, m_deleter)
	{
		m_pMtx.get()->lock();
		std::cout << "locked" << std::endl;
	}

	~MyLockerUncopy()
	{
		std::cout << "unlocked" << std::endl;
	}

private:
	MyLockerUncopy(MyLockerUncopy&) = delete;
	MyLockerUncopy& operator= (const MyLockerUncopy&) = delete;

	MyLockerDeleter m_deleter;//����һ��ɾ������������ȫ�ֺ������ߺ�������
	std::shared_ptr<mutex> m_pMtx;

};