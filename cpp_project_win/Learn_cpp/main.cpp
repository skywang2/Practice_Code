#include "main.h"
#include <ctime>
#include <iostream>
#include "Circular_Queue.hpp"
#include "Queue.hpp"
#include "MyLocker.hpp"
#include "Tree1.cpp"
#include "Regex.hpp"

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
	TestEmplace(const TestEmplace&& t) noexcept
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
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
	catch (...)
	{
		cout << "unknow error." << endl;
	}
	
	system("pause");
	return 0;
}
