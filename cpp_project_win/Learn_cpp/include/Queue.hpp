#include<iostream>
#include<vector>
using namespace std;

class MyQueue {
private:
	vector<int> data;
	int p_start;
public:
	MyQueue() { p_start = 0; }
	bool isEmpty() {
		return p_start >= data.size();
	}

	bool enQueue(int x) {
		data.push_back(x);
		return true;
	}

	bool deQueue() {
		if (isEmpty()) {
			return false;
		}
		p_start++;
		return true;
	}

	int Front() {
		return data[p_start];
	}
};