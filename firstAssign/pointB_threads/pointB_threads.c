#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>


int main (int argc, char *argv[]) {
        int fildes[2];  // declaration of the file descriptors array    
        char pipe_fd0[20], pipe_fd1[20];  // declaration of the corrispective char array in order to pass 
        char char_pid_child1[20];
        int pipe_return;
        int child1_exec_return, child2_exec_return;
        pid_t fork_pid_child1, fork_pid_child2;  

        pipe_return = pipe(fildes);  // here the pipe is created and the file descriptors are passed to fildes
        if (pipe_return < 0)
        {
                perror("pipe");
                return -1;
        }
        printf("\nPIPE FILE DESCRIPTORS");
        fflush(stdout);
        printf("\n   fildes[0] --> %d  reading file descriptor", fildes[0]);            
        fflush(stdout);
        printf("\n   fildes[1] --> %d  writing file descriptor\n\n", fildes[1]);
        fflush(stdout);

        fork_pid_child1 = fork();  //child process 1 is created through forking
        sprintf(char_pid_child1, "%d", fork_pid_child1);  // convert the pid of chld process 1 into a char array

        sprintf(pipe_fd0,"%d", fildes[0]);  // file descriptors are converted to array
        sprintf(pipe_fd1,"%d", fildes[1]);  // of chars in odrder to passa them to char **argv



        if (fork_pid_child1 < 0)  // fork error handling
        {
                perror("fork");
                return -1;
        }

        //============= child1 process ====================

        if (fork_pid_child1 == 0) {                         
                argv[0] = "output_threads";
                argv[1] = pipe_fd0;  // putting the char format file descriptors in the pointer to array of char (char **argv) 
                argv[2] = pipe_fd1;  
                argv[3] = NULL;  // by definition the last member must be NULL 
                child1_exec_return = execve("output_threads", argv, NULL);  // executing the program called from_input superimposing its process to the one of the child   
                if (child1_exec_return < 0)
                {
                        perror("execve");
                        return -1;
                }
        }

        //============ end child process 1 code ===========

        fork_pid_child2 = fork();  // child process 2 is created through forking by the father process


        //=============== child2 process =====================

        if (fork_pid_child2 < 0)  // fork error handling
        {
                perror("fork");
                return -1;
        }

        if (fork_pid_child2 == 0) 
        {                         
                argv[0] = "from_input_threads";
                argv[1] = pipe_fd0;  // putting the char format file descriptors in the pointer to array of char (char **argv) 
                argv[2] = pipe_fd1;  
                argv[3] = char_pid_child1;
                argv[4] = NULL;  // by definition the last member must be NULL 
                printf("char_pid_child1 -->  %s\n", char_pid_child1); fflush(stdout);
                child2_exec_return = execve("from_input_threads", argv, NULL);  // executing the program called output
                if (child2_exec_return < 0)
                {                         
                        perror("execve");        
                        return 1;
                }
        }

        return 0;
}
