#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <set>
using namespace std;

//HJ=huawei judge

int HJ2() {
	vector<long> mi;
	cout << sizeof(long) << endl;
	cout << sizeof(long long) << endl;

	mi.push_back(1);
	mi.push_back(2);
	mi.push_back(4);
	for (int i = 3; i < 50; i++) {
		mi.push_back(mi[i - 1] + mi[i - 2] + mi[i - 3]);
	}
	string in;
	while (cin >> in) {
		string out;
		for (int i = 0; i < in.size(); i++) {
			int temp = (in[i] - 'a' + 1 + mi[i]) % 26;
			cout << i << ", " << mi[i] << ", " << temp << endl;
			char s = temp - 1 + 'a';
			if (temp == 0) s = 'z';
			out.push_back(s);
		}
		cout << out << endl;
	}

	return 0;
}

void HJ14(int num) {
	multiset<string> txt;
	string in;
	while (getline(cin, in)) {
		txt.insert(in);
	}
	for (auto out : txt) {
		cout << out << endl;
	}
}
