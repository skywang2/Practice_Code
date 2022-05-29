#include <unistd.h>
#include <signal.h>
#include <cstdlib>
#include <iostream>

using namespace std;

void KillF(int sig) {
    if(sig > 0) {
        signal(sig, SIG_IGN);
        signal(SIGINT, SIG_IGN);
        signal(SIGTERM, SIG_IGN);
        cout << __FUNCTION__ << endl;
    }
    kill(0, SIGTERM);
    exit(0);
}

int main() {
    cout << "start" << endl;
    if(fork() > 0) {
        signal(SIGTERM, KillF);
    }
    sleep(30);
/*     for(int i = 0; i < 3; i++) {
        cout << __TIME__ << endl;
        sleep(3);
    } */
    cout << "end" << endl;
    return 0;
}