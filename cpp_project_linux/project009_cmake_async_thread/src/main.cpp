#include <iostream>
#include <thread>
#include <unistd.h>
using std::thread;
using std::cout;
using std::endl;

void ThreadFunc(int& a)
{
  cout << "thread a:" << ++a << endl;
}

int main(int argc, char* argv[])
{
  std::cout << "hello world." << std::endl;
  int a = 5;
  thread t(ThreadFunc, std::ref(a));
  if(t.joinable()) t.join();
  cout << "main a:" << a << endl;

  return 0;
}
