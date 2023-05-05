#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include <exception>
#include <condition_variable>
#include <deque>
#include <future>
#include <tuple>
#include <random>
#include <sstream>

using namespace std;

std::atomic<bool> ready(false);
std::mutex mtx;
std::condition_variable cond;
std::condition_variable cond_not_full;
std::condition_variable cond_not_empty;
std::deque<int> box;

void YieldTest(int id)
{
	while (!ready)
	{
		std::lock_guard<mutex> lck(mtx);
		std::this_thread::yield();
		cout << id << " is not ready." << endl;
	}
	
	{
		std::lock_guard<mutex> lck(mtx);
		cout << id << " is ready." << endl;
	}
	std::this_thread::sleep_for(std::chrono::seconds(5));
}

void ThreadTest01()
{
	thread th[100];

	cout << "=====start=====" << endl;
	for (int i = 0; i < 100; ++i)th[i] = thread(YieldTest, i);
	//std::this_thread::sleep_for(std::chrono::seconds(2));
	ready = true;
	for (auto& t : th)
		if (t.joinable())
			t.join();
	cout << "=====end=====" << endl;
	system("pause");
}

std::once_flag flag2;
int intoCount = 0;
void OnceFunc(bool do_throw)
{
	if (do_throw) {
		//std::this_thread::sleep_for(std::chrono::seconds(20));
		throw std::logic_error("parameter is true");
	}
	unique_lock<mutex> lck(mtx);
	cout << "Get in OnceFunc()" << endl;
	intoCount++;
	lck.unlock();
}

void may_throw_function(bool do_throw)
{
	if (do_throw) {
		std::cout << "11111\n"; //所有do_once参数为true的线程都走到这里
		throw std::exception("parameter is true");
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(5));
	std::cout << "22222\n"; //只有一个do_once参数为false的线程能走到这里
}

void do_once(bool do_throw)
{
	try {
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		//std::call_once(flag2, may_throw_function, do_throw);
		std::call_once(flag2, OnceFunc, do_throw);
	}
	catch (logic_error& e) {
		unique_lock<mutex> lck(mtx);
		cout << "system_error " << e.what() << endl;
	}
	catch (system_error& e) {
		unique_lock<mutex> lck(mtx);
		cout << "system_error " << e.what() << endl;
	}
	catch (exception& e) {
		unique_lock<mutex> lck(mtx);
		cout << e.what();
	}
	catch (...) {
		cout << "unknown exception." << endl;
	}

	unique_lock<mutex> lck(mtx);
	cout << ", thread id:" << std::this_thread::get_id() << " flag:" << flag2._Flag << endl;
	return;
}

void ThreadTest02()
{
	thread th[100];

	for (int i = 0; i < 99; ++i)
	{
		th[i] = thread(do_once, true);
	}
	th[99] = thread(do_once, false);
	for each (auto &t in th)
	{
		if (t.joinable())
			t.join();
	}

	cout << "At end," << " intoCount:" << intoCount << " flag:" << flag2._Flag << endl;
}

//生产者
void Productor()
{
	int data = 0;
	while (1)
	{
		std::unique_lock<mutex> lck(mtx);
		cout << "input:";
		cin >> data;
		box.push_back(data);
		lck.unlock();
		cond.notify_one();
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
		//std::this_thread::yield();
	}
}

//消费者
void Consumer()
{
	int data = 0;
	std::unique_lock<mutex> lck(mtx);
	while (1 != data)
	{
		while (box.empty())
		{
			cout << "consumer wait." << endl;
			cond.wait(lck);
			cout << "consumer ok." << endl;
		}
		data = box.front();
		box.pop_front();
		cout << "consumer:" << data << endl;
	}
}

void ThreadTest03()
{
	std::thread c = thread(Consumer);
	std::this_thread::sleep_for(std::chrono::milliseconds(1));
	std::thread p = thread(Productor);

	p.join();
	c.join();
}

int MyAsyncThread01(std::promise<int>& pro)
{
	std::this_thread::sleep_for(std::chrono::seconds(5));
	unique_lock<mutex> lck(mtx);
	cout << "5s";
	lck.unlock();

	int ret = 100;
	pro.set_value(ret);
	return ret;
}

int MyAsyncThread02(std::promise<int>& pro)
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 10);
	stringstream ss;
	int temp = 0, id = -1;

	//id
	std::this_thread::get_id()._To_text(ss);
	ss >> id;
	//sleep
	temp = distribution(generator);
	std::this_thread::sleep_for(std::chrono::seconds(temp));
	//set
	//try
	//{
		cout << "pro addr:" << &pro << endl;
		pro.set_value_at_thread_exit(id);
	//}
	//catch (exception e)
	//{
	//	unique_lock<mutex> lck(mtx);
	//	cout << "id:" << id << "error:" << e.what() << endl;
	//}

	return 0;
}

//测试异步线程操作
void ThreadTest04()
{

	vector<thread*> ths;
	vector<promise<int> > pros;
	vector<future<int> > fus;
	for (int i = 0; i < 20; ++i)
	{
		std::promise<int> pro;
		std::future<int> fu = pro.get_future();
		fus.push_back(move(fu));
		pros.push_back(move(pro));
		thread *th = new thread(MyAsyncThread02, std::ref(pros.back()));
		ths.push_back(th);
	}

	//std::promise<int> pro;
	//std::promise<int> pro1;
	//std::promise<int> pro2;
	//cout << "temp pro addr:" << &pro << endl;
	//cout << "temp pro1 addr:" << &pro1 << endl;
	//cout << "temp pro2 addr:" << &pro2 << endl;

	//std::future<int> fu = pro.get_future();
	//std::future<int> fu1 = pro1.get_future();
	//std::future<int> fu2 = pro2.get_future();
	//cout << "temp fu addr:" << &pro << endl;
	//cout << "temp fu1 addr:" << &pro1 << endl;
	//cout << "temp fu2 addr:" << &pro2 << endl;

	//pros.push_back(move(pro));
	//pros.push_back(move(pro1));
	//pros.push_back(move(pro2));

	while (!fus.empty())
	{
		{
			unique_lock<mutex> lck(mtx);
			cout << "fus size:" << fus.size() << " ";
		}

		for (auto i = fus.begin(); i != fus.end();)
		{
			int tmp = -1;
			std::chrono::microseconds span(10000);
			if (i->valid() && i->wait_for(span) == std::future_status::ready)
			{
				tmp = i->get();
				fus.erase(i++);
				unique_lock<mutex> lck(mtx);
				cout << "fu:" << tmp << endl;
			}
			else
			{
				++i;
				unique_lock<mutex> lck(mtx);
				cout << "。";
			}
		}//for

		{
			unique_lock<mutex> lck(mtx);
			cout << endl;
		}
	}//while

	for (auto th: ths)
		th->join();
}

typedef struct _AFILE
{
	int8_t buff[100];
	int32_t size;
}AFILE;

template<typename T>
void MyMutexTestLocked(T *input)
{
	//test1，测试指针能否指向别处
	input = nullptr;

	//test2，测试能否修改指针所指内存中的数据
	input->size = 100;//实测无法修改被锁的内存
}

template<typename T>
void MyMutexTestTryUse(T *input)
{
	{
		std::lock_guard lck(mutex);
		while (true)
		{
			input;
		}
	}
}

//测试mutex锁住的是指针还是指针所指的内存，亦或都锁住
void ThreadTest05()
{
	AFILE *p = new AFILE();
	thread *locked = new thread(&MyMutexTestLocked<AFILE>, p);
	thread *tryUse = new thread(&MyMutexTestLocked<AFILE>, p);

	if (locked->joinable())
		locked->detach();
	if (tryUse->joinable())
		tryUse->detach();

}

void MyLockFuncOutput()
{
	int i = 0;
	while (i < 10)
		std::cout << "threadid:0x" << std::this_thread::get_id() << ", i:" << i++ << std::endl;
}

//6.锁后有函数调用，能否锁住整个函数
void MyLockWholeFunc()
{
	std::lock_guard<mutex> lck(mtx);
	MyLockFuncOutput();
}

void ThreadTest06()
{
	vector<thread> threads(3);
	for (thread& t : threads)
	{
		t = thread(MyLockWholeFunc);
	}
	for (thread& t : threads)
	{
		t.join();
	}
}

int main()
{
	//ThreadTest01();
	//ThreadTest02();
	//ThreadTest03();
	//ThreadTest04();
	//ThreadTest05();
	ThreadTest06();

	system("pause");
	return 0;
}
