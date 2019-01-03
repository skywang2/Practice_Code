#include<iostream>
using namespace std;

class MyCircularQueue {
private:
	int *x;
	int front, rear, length;

public:
	MyCircularQueue(int k){
		x = new int[k + 1];
		length = k + 1;
		front = 0;	//参考答案用的-1，因为它在后面插入时先front自增，再赋值
		rear = 0;
	}

	~MyCircularQueue() {
		delete[] x;
	}

	bool isEmpty() {
		return front == rear;
	}

	bool isFull() {
		return (rear + 1) % length == front;
	}

	bool enQueue(int value) {
		if (isFull())
			return false;
		x[rear] = value;
		rear = (rear + 1) % length;
		return true;
	}

	bool deQueue() {
		if (isEmpty())
			return false;
		front = (front + 1) % length;
		return true;
	}

	int Front() {
		if (isEmpty())
			return -1;
		return x[front];
	}

	int Rear() {
		if (isEmpty())
			return -1;
		return rear==0?x[length-1]:x[rear-1];
	}
};