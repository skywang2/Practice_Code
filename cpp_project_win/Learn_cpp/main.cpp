#include "main.h"
#include <ctime>
#include <iostream>
#include <random>
#include "Circular_Queue.hpp"
#include "Queue.hpp"
#include "MyLocker.hpp"
#include "Tree1.cpp"
#include "Regex.hpp"
#include "TestMemberData.h"
#include "forward.hpp"
#include "VisitorMode.hpp"

std::mutex g_mtx;

class TestEmplace
{
public:
	TestEmplace(const std::string s, int i) 
		: m_s(s) , m_i(i)
	{ 
		std::cout << __FUNCTION__ << " raw" << std::endl; 
	}
	TestEmplace(const TestEmplace& t) 
	{
		m_s = t.m_s;
		m_i = t.m_i;
		std::cout << __FUNCTION__ << " copy" << std::endl;
	}
	TestEmplace(TestEmplace&& t) noexcept
		: m_s(std::move(t.m_s)), m_i(t.m_i)
	{
		std::cout << __FUNCTION__ << " move" << std::endl;
	}
private:
	std::string m_s;
	int m_i;
};

void test_circularQueue() {
	MyCircularQueue circularQueue(3);
	circularQueue.enQueue(1);	//插入第一位
	circularQueue.enQueue(2);	//插入第二位
	circularQueue.enQueue(3);	//插入第三位
	circularQueue.enQueue(4);	//位置满，无法插入
	cout << "Rear:" << circularQueue.Rear() << endl;
	circularQueue.isFull();
	circularQueue.deQueue();
	circularQueue.enQueue(4);
	cout << "Rear:" << circularQueue.Rear() << endl;

	cin.get();
}

void test_queue() {
	MyQueue q;
	q.enQueue(66);
	q.enQueue(3);
	q.enQueue(7);
	while (!q.isEmpty())
	{
		if (!q.isEmpty()) {
			cout << q.Front() << " ";
		}
		q.deQueue();
	}
	if(!q.isEmpty())
		cout << q.Front() << endl;

	cin.get();
}

void test_Tree() {
	int i;
	Solution tree1;
	TreeNode *root = NULL;
	cout << "Create a tree" << endl;
	root = tree1.createTree();
	cout << "Created" << endl;

	cout << "++++++++++++++++++++++" << endl;
	cout << "Output the preorder" << endl;
	vector<int> preorder = tree1.preorderTraversal(root);
	i = 0;
	while (i<preorder.size()) {
		cout << preorder[i] << ' ';
		i++;
	}
	cout << endl;

	cout << "++++++++++++++++++++++" << endl;
	cout << "Output the inorder" << endl;
	vector<int> inorder = tree1.inorderTraversal(root);
	i = 0;
	while (i<inorder.size()) {
		cout << inorder[i] << ' ';
		i++;
	}
	cout << endl;

	system("pause");
}

void test_level_tree() {
	Solution tree1;
	TreeNode *root = NULL;
	cout << "Create a tree" << endl;
	root = tree1.createTree();
	cout << "Created" << endl;
	tree1.levelOrder(root);

	//system("pause");
}

const int& GetConstRefValue()
{
	int a = 10;
	cout << "&a:" << &a << endl;
	return a;
}

int main(int argc, char* argv[]) {
	builtInQueue();//简单使用队列
	//test_Tree();
	//test_level_tree();//创建树并广度遍历（层序遍历）

	try
	{
		{
			cout << "-----MyLockerUncopy" << endl;
			MyLockerUncopy mylocker01(&g_mtx);//该类不能拷贝，初始化时加锁，析构时解锁
		}

		//返回局部变量的const引用，这是错误的，会产生未初始化的变量，得到0xcccccccc（-858993460）
		cout << "-----return const reference intger" << endl;
		const int& p = GetConstRefValue();
		cout << "p:" << p << endl;
		cout << "&p:" << &p << endl;

		//使用STL中的正则表达式类
		cout << "-----using regex of STL" << endl;
		RegexMatch();
		RegexMatchResult();
		RegexSearch();
		RegexTokenize();
		RegexReplace();

		//vector, emplace()/emplace_back()
		{
			vector<TestEmplace> vec1, vec2;
			//push_back
			vec1.reserve(100);
			clock_t start = clock();
			//for (int i = 0; i < 1000; ++i)
				vec1.push_back({ std::string("hello world"), 1 });
			clock_t end = clock();
			cout << "push_back time:" << (end - start)/100.0 << endl;
			//emplace_back
			vec2.reserve(100);
			start = clock();
			//for (int i = 0; i < 1000; ++i)
				vec2.emplace_back(std::string("hello world"), 1);
			end = clock();
			cout << "emplace_back time:" << (end - start) / 100.0 << endl;
		}

		//static_assert

		//9.pImpl
		{
			TestMemberData mData(0);
		}

		//10.forward
		{
			std::cout << std::endl << "10.forward" << std::endl;
			std::string val("1234");

			std::cout << "using std::string" << std::endl;
			MoreThanProcess(val);
			std::cout << "using std::move(std::string)" << std::endl;
			MoreThanProcess(std::move(val));
		}

		//11.计算vector长度
		{
			std::vector<float> ff;
			ff.push_back(1.0);
			ff.push_back(1.0);
			ff.push_back(1.0);
			size_t size_float = sizeof(float);
			size_t size_vector = sizeof(ff);//注意这不是3*sizeof(float)
		}

		//12.访问者模式
		{
			ObjectStructure objects;
			std::shared_ptr<ConcreteElementA> pA(new ConcreteElementA);
			std::shared_ptr<ConcreteElementB> pB(new ConcreteElementB);

			objects.Attach(pA);
			objects.Attach(pB);

			ConcreteVisitor1 v1;
			ConcreteVisitor2 v2;

			objects.Accept(&v1);//所有元素使用1号类中的处理方法
			objects.Accept(&v2);//所有元素使用2号类中的处理方法
		}
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "unknow error." << endl;
	}
	
	//测试*a++的执行顺序
	{
		int arr[2] = { 3, 7 };
		int* p = arr;

		int t = *p++;//*与++优先级相同，并且“从右向左”执行，所以等价于*(p++)，t记录p++当前值，p在整句结束后才加1，最后p指向第二个元素，返回7
		int tmp = 0;
	}

	//使用cpp的随机数生成类
	{
		std::random_device rnd;//利用CPU指令生成准随机数（比伪随机好）
		std::mt19937 rng(rnd());//初始化随机数生成器（伪随机）
		std::uniform_int_distribution<int> uni(0, 100);//使用不同概率分布对象，完成类似x % 10的取随机数范围的操作
		std::uniform_real_distribution<double> und(0, 1.0);
		int numi = uni(rng);
		double numd = und(rng);
	}

	system("pause");
	return 0;
}
