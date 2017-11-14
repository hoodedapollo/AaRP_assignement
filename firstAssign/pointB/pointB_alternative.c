#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

 
int main (int argc, char *argv[]) {
    int fildes[2];      // declaration of the file descriptors array    
    char pipe_fd0[20], pipe_fd1[20];        // declaration of the corrispective char array in order to pass 
    int w[10];
    ssize_t bytes_read[10];     // array which stores number of bytes read during each reading

// here the pipe is created and the file descriptors are passed to fildes

    pipe(fildes);
    perror("pipe");
    printf("\nfildes[0] --> %d", fildes[0]);            
    fflush(stdout);
    printf("\nfildes[1] --> %d\n\n", fildes[1]);
    fflush(stdout);
    
    pid_t fork_pid = fork();        // a child process is created through forking
    
 // file descriptors are converted to array  of chars in odrder to passa them to char **argv
           
    sprintf(pipe_fd0,"%d", fildes[0]);                  
    sprintf(pipe_fd1,"%d", fildes[1]);                  
    
    if (fork_pid < 0)       // fork error handling
        perror("fork");

// child process code

    if (fork_pid == 0) {                               
        argv[0] = "from_input";     // by definition the first element must be the bin namefile
        argv[1] = pipe_fd0;     // putting the char format file descriptors in the pointer to array of char (char **argv) 
        argv[2] = pipe_fd1;    
        argv[3] = NULL;      // by definition the last member must be NULL 
        execve("from_input", argv, NULL);       // overlaing the current process with the from_imput program process   
        perror("execve");
    }
    
// father process code 

    pid_t wait_pid = wait(NULL);        // father process waits until the child process exit 
    printf("\npid from wait -->  %d", wait_pid);
    close(fildes[1]);       // closing the writing side of the pipe relative to the father process

    printf("\noutput fildes[0] --> %d", fildes[0]);
    size_t w_size = sizeof(w[0]);
    printf("\nsize of w[i] -- >  %ld", w_size);
    printf("\nbytes read: ");

// reading from pipe one one integer per time and storing it in w[i]

    for (int i = 0; i<10; i++) {
        bytes_read[i] = read(fildes[0], &w[i], w_size);
        perror("read");
        printf(" %ld",bytes_read[i]);
    }

    close(fildes[0]);       // closing the reading side of the pipe relative to the father process

// bubble sort of the array containing the integers

    for(int j = 0; j<10; j++) {
        for(int i=0;i<9;i++) {
            if (w[i] > w[i+1]) {
            int temp = w[i];
            w[i] = w[i+1];
            w[i+1] = temp;
            }
        }
    }
    printf("\nThe ordered vector is\n[");

    for (int i=0;i<10;i++) {
        printf(" %d", w[i]);
    }
    printf(" ]\n");

    exit(EXIT_SUCCESS);
    return 0;
}     
