#include<iostream>
#include<queue>
using namespace std;

void builtInQueue() {
	queue<int> q;

	q.push(2);
	q.push(3);
	q.push(345);

	cout << "size:" << q.size() << endl;
	cout << "first:" << q.front() << endl;
	cout << "last:" << q.back() << endl;

	//cin.get();
}