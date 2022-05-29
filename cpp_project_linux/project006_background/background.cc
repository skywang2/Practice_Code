#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int main() {
    if(fork() > 0) {
        return 0;
    }
    
    while(1) {
        cout << "this is background program." << endl;
        sleep(3);
    }
    return 0;
}