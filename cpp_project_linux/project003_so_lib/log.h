#ifndef LOG_H
#define LOG_H

#include <string>

using std::string;

class MyLoger {
 public:
  MyLoger();
  ~MyLoger();
 
  const string& GetCurrentFuncLine(string& out, const char* func, int line);
 private:
  int size;
};

#endif