#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

 
int main (int argc, char *argv[]) {
    int fildes[2];                                      // declaration of the file descriptors array    
    char pipe_fd0[20], pipe_fd1[20];                    // declaration of the corrispective char array in order to pass 
    char char_pid_child1[20]; 
    pid_t fork_pid_child2;
    int pipe_ret;

    pipe_ret = pipe(fildes);                                       // here the pipe is created and the file descriptors are passed to fildes
    if (pipe_ret < 0)
    {
            perror("pipe");
            return -1;
    }
    pid_t fork_pid_child1 = fork();                            // a child process is created through forking
    if (fork_pid_child1 < 0)                                   // fork error handling
    {    
        perror("fork child1");
        return -1;
    }
    if (fork_pid_child1 > 0)
            fork_pid_child2 = fork();
    
    sprintf(pipe_fd0,"%d", fildes[0]);                  // file descriptors are converted to array
    sprintf(pipe_fd1,"%d", fildes[1]);                  // of chars in odrder to passa them to char **argv
    
    if (fork_pid_child1 == 0) {                                // child process code
        argv[0] = "from_input_threads_two_childs";                         // by definition the first element must be the bin namefile
        argv[1] = pipe_fd0;                             // putting the char format file descriptors in the pointer to array of char (char **argv) 
        argv[2] = pipe_fd1;                             // by definition the last member must be NULL 
        argv[3] = NULL;
        execve("from_input_threads_two_childs", argv, NULL);               //  
        perror("execve");
    }
    else if (fork_pid_child2 < 0)
    {
        perror("fork child2");
        return -1;
    }

    else if (fork_pid_child2 == 0)
    {
    argv[0] = "output_threads_two_childs";
    argv[1] = pipe_fd0;
    argv[2] = pipe_fd1;
    argv[4] = NULL;
    execve("output_threads_two_childs", argv, NULL);
    perror("execve");
    }
    
    else 
    {
    wait(NULL);
    return 0;
    }     
}
