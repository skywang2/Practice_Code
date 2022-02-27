#include <iostream>
#include <string>
#include "./include/helloworld.h"
#include "./include/log.h"

using std::string;
using std::cout;
using std::endl;

int main(int argc, char* argv[]){
  cout << "It is main()" << endl;
  Util tool;
  tool.GetCurrentTime();

  MyLoger loger;
  string output;
  cout << loger.GetCurrentFuncLine(output, __FUNCTION__, __LINE__);
  
  return 0;
}
