#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointB_lib.h"

int main (int argc, char *argv[]) {
        int fildes[2];                                      // declaration of the file descriptors array    
        char pipe_fd0[20], pipe_fd1[20];                    // declaration of the corrispective char array in order to pass 
        pid_t fork_pid_child1, fork_pid_child2;
        int pipe_ret;

        FILE *f = fopen("pointB.log", "w");  // clear the log file at each execution of the process
        fclose(f);
        pipe_ret = pipe(fildes);  // here the pipe is created and the file descriptors are passed to fildes
        log_func("FATHER pipe");
        err_control(pipe_ret,"FATHER pipe: ",0);

        sprintf(pipe_fd0,"%d", fildes[0]); // file descriptors are converted to array
        sprintf(pipe_fd1,"%d", fildes[1]);  // of chars in odrder to store them in argv

        argv[1] = pipe_fd0;  // putting the char format file descriptors in the pointer to array of char (char **argv) 
        argv[2] = pipe_fd1;  // by definition the last member must be NULL 
        argv[3] = NULL;

        fork_pid_child1 = fork();  // create a child process through forking
        if (fork_pid_child1 != 0)  // father process
        {
                log_func("FATHER first fork");
                err_control(fork_pid_child1,"FATHER first fork: ",0);
                fork_pid_child2 = fork(); // create another child process with the same father as th
        }

        if (fork_pid_child1 == 0)  // child process code
        {
                argv[0] = "child1";  // by definition the first element must be the bin namefile
                int exec_child1_ret = execve("child1", argv, NULL); // superimpose the calling procces with the process of program child1
                err_control(exec_child1_ret,"FATHER child1 exec: ",0);

        }
        else if (fork_pid_child2 != 0) // father process
        {
                log_func("FATHER second fork");
                err_control(fork_pid_child1,"FATHER second fork: ",0);
        }

        else if (fork_pid_child2 == 0)
        {
                argv[0] = "child2";
                int exec_child2_ret = execve("child2", argv, NULL);
                err_control(exec_child2_ret,"FATHER child2 exec: ",0);
        }
        if (fork_pid_child1 != 0 && fork_pid_child2 != 0) // father process
        {
                int first_wait_ret = wait(NULL); // wait for one of the children
                log_func("FATHER first wait");
                err_control(first_wait_ret,"FATHER first wait: ",0);

                int second_wait_ret = wait(NULL); // wait for the other child
                log_func("FATHER second wait");
                err_control(second_wait_ret,"FATHER second wait: ",0);

                return 0;
        }
}

