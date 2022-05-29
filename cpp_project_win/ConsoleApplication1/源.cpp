#include <list>
#include <iostream>
#include <Windows.h>
using namespace std;

#define M 3
bool judge(list<int> in, list<int> key) {
	int temp1[M] = { 0 }, temp2[M] = { 0 };
	int j = 0;
	for (list<int>::iterator i = in.begin(); i != in.end(); i++, j++)
		temp1[j] = *i;

	j = 0;
	for (list<int>::iterator i = key.begin(); i != key.end(); i++, j++)
		temp2[j] = *i;

	int flag = 0;
	for (j = 0; j < M; j++) {
		if (temp1[j] == temp2[j])
			flag = 1;
		else
			flag = 0;
	}
		

	if (flag) {
		return true;
	}
	return false;
}

int main() {
	list<int> in, key;
	int temp = 0;
	
	for (int i = 0; i < M; i++) {
		cin >> temp;
		key.push_back(temp);
	}
		


	while (true) {
		if(in.size() == M)
			in.pop_front();
		cin >> temp;
		in.push_back(temp);
		if (judge(in, key))
			break;
	}

	cout << "ok" << endl;
	system("pause");
	return 0;
}