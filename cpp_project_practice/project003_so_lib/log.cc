#include "log.h"
#include <iostream>
#include <sstream>

using std::cout;
using std::endl;
using std::stringstream;

//
MyLoger::MyLoger() {
    cout << "construct MyLoger" << endl;
}

//
MyLoger::~MyLoger() {
    cout << "deconstruct MyLoger" << endl;
}

//
const string& MyLoger::GetCurrentFuncLine(string& out, const char* func, int line) {
  stringstream func_line(out);
  func_line << func << ", " << line;
  return out;
}
