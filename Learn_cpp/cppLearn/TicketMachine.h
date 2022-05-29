#pragma once
class TicketMachine
{
public:
	TicketMachine();
	~TicketMachine();
	void showPrompt();
	void insertMoney(int money);
	void showBalence();
	void printTicket();
	void showTotal();
private:
	const int PRICE;
	int balance;
	int total;
};