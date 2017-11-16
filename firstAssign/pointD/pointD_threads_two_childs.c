#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
int err_control (int func_ret, char perror_msg[], int err_value);

int main (int argc, char *argv[]) {
        int fildes_firstpipe[2], fildes_secondpipe[2];                                     // declaration of the file descriptors array    
        char firstpipe_fd0[20], firstpipe_fd1[20];                    // declaration of the corrispective char array in order to pass 
        char secondpipe_fd0[20], secondpipe_fd1[20];
        pid_t fork_pid_child1, fork_pid_child2;
        int firstpipe_ret, secondpipe_ret;
        int exec_child1_ret, exec_child2_ret;
        int first_wait_return, second_wait_return;
        signal(SIGUSR1, SIG_IGN);
        signal(SIGUSR2,SIG_IGN);
        firstpipe_ret = pipe(fildes_firstpipe);                                       // here the pipe is created and the file descriptors are passed to fildes
        err_control(firstpipe_ret,"first pipe",0);  // return handling with perror

        sprintf(firstpipe_fd0,"%d", fildes_firstpipe[0]);                  // file descriptors are converted to array
        sprintf(firstpipe_fd1,"%d", fildes_firstpipe[1]);                  // of chars in odrder to passa them to char **argv

        secondpipe_ret = pipe(fildes_secondpipe);
        err_control(secondpipe_ret,"second pipe",0);  // return handling with perror
        
        sprintf(secondpipe_fd0,"%d", fildes_secondpipe[0]);                  // file descriptors are converted to array
        sprintf(secondpipe_fd1,"%d", fildes_secondpipe[1]);                  // of chars in odrder to passa them to char **argv

        fork_pid_child1 = fork();                            // a child process is created through forking
        err_control(fork_pid_child1, "fork child 1",0);  // fork error handling with perror
        
        if (fork_pid_child1 > 0)
                fork_pid_child2 = fork();
        err_control(fork_pid_child2,"fork child2", 0);
        
                argv[1] = firstpipe_fd0;                             // putting the char format file descriptors in the pointer to array of char (char **argv) 
                argv[2] = firstpipe_fd1;                             // by definition the last member must be NULL 
                argv[3] = secondpipe_fd0;
                argv[4] = secondpipe_fd1; 
                argv[5] = NULL;

        if (fork_pid_child1 == 0) {                                // child process code
                argv[0] = "from_input_threads_two_childs";                         // by definition the first element must be the bin namefile
                exec_child1_ret = execve("from_input_threads_two_childs", argv, NULL);               //  
                err_control(exec_child1_ret,"execve child1",0);
        }

        else if (fork_pid_child2 == 0)
        {
                argv[0] = "output_threads_two_childs";
                exec_child2_ret = execve("output_threads_two_childs", argv, NULL);
                err_control(exec_child2_ret,"execve child2",0);
        }

        else 
        {
               printf("child1 pid --> %d\n", fork_pid_child1); fflush(stdout);
               printf("child2 pid --> %d\n", fork_pid_child2); fflush(stdout);
               first_wait_return = wait(NULL);
               err_control(first_wait_return,"first wait",0);
               printf("firt wait return --> %d\n", first_wait_return); fflush(stdout);
               second_wait_return = wait(NULL);
               err_control(second_wait_return,"second wait",0);
               printf("second wait return --> %d\n", second_wait_return); fflush(stdout);
        }     
        return 0;
}


int err_control (int func_ret, char perror_msg[], int err_value) {
    if (func_ret < err_value)
    {   
        perror(perror_msg);
        return -1;
    }
}
