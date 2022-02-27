#include "helloworld.h"
#include <ctime>
#include <iostream>

using std::cout;
using std::endl;

//
Util::Util()
    : size(-1) {
  cout << "construct Util" << endl;
}

//
Util::~Util(){
  cout << "deconstruct Util" << endl;
}

//
void Util::GetCurrentTime(){
  time_t now = time(0);
  cout << ctime(&now);
  return;
}