#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointA_enanched_lib.h"

void handler1(int signo);

void handler2(int signo);

int main(int argc, char *argv[]) 
{

        FILE *f =fopen("pointA_enanched.log","w");
        fclose(f);
        sighandler_t father_sig1_ret, father_sig2_ret;
        pid_t fork_pid = fork(); 
        if (fork_pid != 0)  // father process
        {

                father_sig1_ret = signal(SIGUSR1,handler1);
                log_func("FATHER SIGUSR1 recieved");
                signal_err_control(father_sig1_ret,"FATHER SIGUSR1 signal:"); 
                father_sig2_ret = signal(SIGUSR2,SIG_IGN);
                log_func("FATHER SIGUSR2 recieved");
                signal_err_control(father_sig2_ret,"FATHER SIGUSR2 signal:"); 
                sleep(1);
                log_func("FATHER first fork");
                err_control(fork_pid,"FATHER fork: ",0);
                pid_t pid_from_wait = wait(NULL);
                log_func("FATHER wait");
                err_control(pid_from_wait,"FATHER wait: ",0);
                if (pid_from_wait == fork_pid)
                {
                        printf("\nFATHER: Well my son is gone, I should go too\n"); fflush(stdout);
                }
                return 0;
        }

        if (fork_pid == 0)
        {
                sighandler_t child_sig1_ret = signal(SIGUSR1,SIG_IGN);
                log_func("CHILD SIGUSR1 recieved");
                signal_err_control(child_sig1_ret,"CHILD SIGUSR1 signal:"); 
                sighandler_t child_sig2_ret = signal(SIGUSR2,handler2);
                log_func("CHILD SIGUSR2 recieved");
                signal_err_control(child_sig2_ret,"CHILD SIGUSR2 signal:"); 
                sleep(1);
                printf("\nCHILD: Hi father, may I go to bed?\n"); fflush(stdout);
                int child_kill_ret = kill(0,SIGUSR1);
                log_func("CHILD SIGUSR1 sent");
                err_control(child_kill_ret,"CHILD SIGUSR1 kill",0);
                sleep(5);

        }
}

void handler1(int signo)
{
        printf("\nFATHER: Sure son, good night\n"); fflush(stdout);
        int father_handler_kill_ret = kill(0,SIGUSR2);
        log_func("FATHER SIGUSR2 sent");
        err_control(father_handler_kill_ret,"FATHER SIGUSR2 kill",0);
}

void handler2(int signo)
{
        printf("\nCHILD:Good night\n"); fflush(stdout);
        exit(EXIT_SUCCESS);
}
