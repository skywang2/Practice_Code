#pragma once
class NumberDisplay
{
public:
	NumberDisplay();
	~NumberDisplay();
	bool increase();
private:
	int limit;
	int value;
};

