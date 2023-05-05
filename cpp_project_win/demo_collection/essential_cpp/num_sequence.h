#pragma once
#include <vector>
#include <iostream>
using std::vector;
using std::cout;
using std::endl;

//#ifndef PRINT()
#define PRINTS() (cout << __FUNCTION__ << endl)
//#endif

class num_sequence
{
public:
	num_sequence();
	~num_sequence();
	typedef void(num_sequence::*PtrType)(int);

	void func1(int){ PRINTS(); };
	void func2(int){ PRINTS(); };
	void func3(int){ PRINTS(); };
	void func4(int){ PRINTS(); };
	void func5(int){ PRINTS(); };
	void func6(int){ PRINTS(); };

private:
	vector<int>* _elem;
	PtrType _pmf;
	static const int num_seq = 7;
	static PtrType func_tbl[num_seq];
	static vector<vector<int> > seq;
};

num_sequence::num_sequence()
{
}

num_sequence::~num_sequence()
{
}

typedef num_sequence::PtrType PtrType;
PtrType num_sequence::func_tbl[num_seq] =
{
	0,
	&num_sequence::func1,
	&num_sequence::func2,
	&num_sequence::func3,
	&num_sequence::func4,
	&num_sequence::func5,
	&num_sequence::func6,
};
