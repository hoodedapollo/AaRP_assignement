#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointA_lib.h"

void handler1(int signo);

void handler2(int signo);

int main(int argc, char *argv[]) 
{

        FILE *f =fopen("pointA.log","w");
        fclose(f);

        pid_t fork_pid = fork();
        if (fork_pid != 0)  // father process
        {
                
                signal(SIGUSR1,handler1);
                signal(SIGUSR2,SIG_IGN);
                sleep(1);
                log_func("FATHER first fork");
                err_control(fork_pid,"FATHER fork: ",0);
                pid_t pid_from_wait = wait(NULL);
                log_func("FATHER wait");
                err_control(pid_from_wait,"FATHER wait: ",0);
                if (pid_from_wait == fork_pid)
                {
                printf("\nFATHER: Well, I should go too\n"); fflush(stdout);
                }
        }

        if (fork_pid == 0)
        {
                signal(SIGUSR1,SIG_IGN);
                signal(SIGUSR2,handler2);
                sleep(1);
                printf("\nCHILD: Hi father, may I go to bed?\n"); fflush(stdout);
                kill(0,SIGUSR1);
                sleep(5);

        }
        return 0;
}

void handler1(int signo)
{
printf("\nFATHER: Sure son, good night\n"); fflush(stdout);
kill(0,SIGUSR2);
}

void handler2(int signo)
{
printf("\nCHILD:Good night\n"); fflush(stdout);
exit(EXIT_SUCCESS);
}
