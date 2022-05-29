#include <iostream>
#include <future>
#include <memory>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include <mutex>
#include <chrono>
#include <deque>
#include <random>
using namespace std;

std::mutex mtx;
default_random_engine rdGenerator;
uniform_int_distribution<int> distribute(1, 10);

int FromIDToInteger(const std::thread::id& id)
{
	int iID = -1;
	std::stringstream ss;

	ss << id;
	ss >> iID;

	return iID;
}

//线程函数
int MyAsyncFunc01(promise<int>& pro)
{
	int iID = -1, span = 0;
	thread::id id;

	span = distribute(rdGenerator);
	this_thread::sleep_for(std::chrono::seconds(span));
	id = this_thread::get_id();
	iID = FromIDToInteger(id);
	pro.set_value(iID);

	return 0;
}

// [comment]
// 多异步线程
// > I 甩手速算
// [/comment]
void ThreadTest01()
{
	const int threadCount = 10;
	vector<thread*> ths;
	vector<promise<int> > pros;
	vector<future<int> > fus;
	for (int i = 0; i < threadCount; ++i)
	{
		promise<int> pro;
		future<int> fu = pro.get_future();
		fus.push_back(move(fu));
		pros.push_back(move(pro));
	}
	//必须把thread创建与promise创建分开，否则会出现线程函数promise内指针为空的问题，原因未知
	for (int i = 0; i < threadCount; ++i)
	{
		thread* th = new thread(MyAsyncFunc01, ref(pros[i]));
		ths.push_back(th);
	}
	while (!fus.empty())
	{
		{
			unique_lock<mutex> lck(mtx);
			cout << ". ";
		}
		for (auto i = fus.begin(); i != fus.end();)
		{
			auto span = std::chrono::milliseconds(200);
			if (i->valid() && i->wait_for(span) == future_status::ready)
			{
				int id = i->get();
				unique_lock<mutex> lck(mtx);
				i = fus.erase(i);
				cout << "\n[id:" << id << "]\n";
			}
			else
			{
				++i;
			}
		}
	}

	//auto span = std::chrono::milliseconds(6000);
	//if (fus[0].valid() && fus[0].wait_for(span) == future_status::ready)
	//{
	//	int id = fus[0].get();
	//	unique_lock<mutex> lck(mtx);
	//	cout << "id:" << id << endl;
	//}

	for (auto& th : ths)
	{
		th->join();
	}
}

int main(int argc, char* argv[])
{
	ThreadTest01();

	system("pause");
	return 0;
}
 