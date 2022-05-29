#include "TicketMachine.h"
#include<iostream>
using namespace std;


TicketMachine::TicketMachine() : PRICE(0)
{

}


TicketMachine::~TicketMachine()
{
}


void TicketMachine::showPrompt() 
{
	cout << "something" << endl;
}

void TicketMachine::insertMoney(int money) 
{
	balance += money;
}

void TicketMachine::showBalence() 
{
	cout << balance;
}

void TicketMachine::printTicket() 
{
	
}
void TicketMachine::showTotal() 
{

}