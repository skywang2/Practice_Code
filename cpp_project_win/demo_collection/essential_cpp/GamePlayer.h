#pragma once
class GamePlayer
{
public:
	GamePlayer();
	~GamePlayer();
private:
	static const int NumTurns = 5;
	int scores[NumTurns];
	enum MyEnum
	{
		COUNT1 = 2,
		COUNT2 = 3
	};
	int find[COUNT2];
};

