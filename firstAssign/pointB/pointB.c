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
    
    pipe(fildes);                                       // here the pipe is created and the file descriptors are passed to fildes
    perror("pipe");
    printf("\nfildes[0] --> %d", fildes[0]);            
    fflush(stdout);
    printf("\nfildes[1] --> %d\n\n", fildes[1]);
    fflush(stdout);
    
    pid_t fork_pid = fork();                            // a child process is created through forking
    
    
    sprintf(pipe_fd0,"%d", fildes[0]);                  // file descriptors are converted to array
    sprintf(pipe_fd1,"%d", fildes[1]);                  // of chars in odrder to passa them to char **argv
    
    if (fork_pid < 0)                                   // fork error handling
        perror("fork");
    if (fork_pid == 0) {                                // child process code
        argv[0] = "from_input";                         // by definition the first element must be the bin namefile
        argv[1] = pipe_fd0;                             // putting the char format file descriptors in the pointer to array of char (char **argv) 
        argv[2] = pipe_fd1;                             // by definition the last member must be NULL 
        argv[3] = NULL;
        execve("from_input", argv, NULL);               //  
        perror("execve");
    }
    else
    {
    argv[0] = "output";
    argv[1] = pipe_fd0;
    argv[2] = pipe_fd1;
    argv[3] = NULL;
    execve("output", argv, NULL);
    perror("execve");
    }
    return 0;
}     
