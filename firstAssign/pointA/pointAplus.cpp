#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>

using namespace std;

int main () {
    pid_t fork_pid = fork();
    if (fork_pid < 0 ) 
        perror("fork");
    else if (fork_pid == 0) {
        cout << "la fork ha avuto successo " << "\n\n";
        cout << "CHILD PROCESS"<< endl;
        cout << "   fork pid -->  " << fork_pid << endl;
        cout << "   child pid -->  " << getpid() << endl;
        pid_t father_pid = getppid();
        kill(father_pid,SIGUSR1);
        perror("kill");
        exit(EXIT_SUCCESS);
    }
    
    pid_t pid_from_wait = wait(NULL);
    cout << "FATHER PROCESS" << endl;
    cout << "   pid from wait -->  " << pid_from_wait << endl;
    cout << "   fork pid -->  " << fork_pid << endl;
    cout << "   father pid -->  " << getpid() << endl;
    
    return (0);
}   
