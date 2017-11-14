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
    {
        perror("fork");
        return 1;
    }

//============= child process ================================

    if (fork_pid == 0) 
    {
        cout << "\nfork succeded\n" << flush; 
        cout << "CHILD PROCESS"<< endl;
        cout << "   fork pid -->  " << fork_pid << endl << flush;
        cout << "   child pid -->  " << getpid() << endl << flush;
        exit(EXIT_SUCCESS);
    }
    
//============= father process ===============================

    pid_t pid_from_wait = wait(NULL);
    cout << "FATHER PROCESS" << endl;
    cout << "   pid from wait -->  " << pid_from_wait << "  --> it is the child pid" << endl << flush;
    cout << "   fork pid -->  " << fork_pid  << "  --> it is the child pid" << endl << flush;
    cout << "   father pid -->  " << getpid() << endl;
    
    return (0);
}   
