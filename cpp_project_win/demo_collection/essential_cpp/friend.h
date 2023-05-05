#ifndef _FRIEND_H
#define _FRIEND_H

class FriClassIterator;

class FriClass
{
public:
	class FriClassIterator
	{
	public:
		FriClassIterator() :a(0), _index(-1), _values(){};
		FriClassIterator(int idx, int *pos) :a(0), _index(idx), _values(pos){};
		~FriClassIterator(){};
		bool operator==(const FriClassIterator& rhs){ return _index == rhs._index; }
		bool operator!=(const FriClassIterator& rhs){ return !(_index == rhs._index); }
		int operator*(){ return _values[_index]; }
		FriClassIterator &operator++(){ ++this->_index; return *this; }

	private:
		int a;
		int _index;
		int* _values;
	};

public:
	FriClass(int s, int a1=0, int b1=0):a(a1),b(b1),size(s)
	{
		if (size > 0)
		{
			values = new int[size];
			for (int i = 0; i < size; ++i){ values[i] = i; }
		}
	}
	~FriClass()
	{ 
		delete[] values; 
	}
	inline FriClass& operator++();
	inline FriClass operator++(int);
	//friend int FriClassIterator::operator*(const FriClass& rhs);
	typedef FriClassIterator iterator;
	iterator begin() const
	{
		return iterator(0, values);
	}
	iterator end() const
	{
		return iterator(size, values);
	}

	//static int GetAValueStatic() { return a; }
public:
	int b;

private:
	int a;
	int GetAValue(){ return a; }
	int* values;
	int size;
};

FriClass& FriClass::operator++()
{
	++this->a;
	return *this;
}

FriClass FriClass::operator++(int)
{
	FriClass obj = *this;
	++obj.a;
	return obj;
}

#endif _FRIEND_H
