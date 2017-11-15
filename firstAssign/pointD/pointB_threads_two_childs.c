#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

 
int main (int argc, char *argv[]) {
    int fildes_firstpipe[2];                                      // declaration of the file descriptors array    
    int fildes_secondpipe[2];
    char firstpipe_fd0[20], firstpipe_fd1[20];                    // declaration of the corrispective char array in order to pass 
    char secondpipe_fd0[20], secondpipe_fd1[20];
    char char_pid_child2[20]; 
    pid_t fork_pid_child2;
    int firstpipe_ret, secondpipe_ret;

    firstpipe_ret = pipe(fildes_firstpipe);                                       // here the pipe is created and the file descriptors are passed to fildes

    
    if (firstpipe_ret < 0)
    {
            perror("first pipe");
            return -1;
    }
    sprintf(firstpipe_fd0,"%d", fildes_firstpipe[0]);                  // file descriptors are converted to array
    sprintf(firstpipe_fd1,"%d", fildes_firstpipe[1]);                  // of chars in odrder to passa them to char **argv

    secondpipe_ret = pipe(fildes_secondpipe);
    if (secondpipe_ret < 0)
    {
            perror("second pipe");
            return -1;
    }
    sprintf(secondpipe_fd0,"%d", fildes_secondpipe[0]);                  // file descriptors are converted to array
    sprintf(secondpipe_fd1,"%d", fildes_secondpipe[1]);                  // of chars in odrder to passa them to char **argv

    pid_t fork_pid_child1 = fork();                            // a child process is created through forking
    if (fork_pid_child1 < 0)                                   // fork error handling
    {    
        perror("fork child1");
        return -1;
    }
    if (fork_pid_child1 > 0)
            fork_pid_child2 = fork();
            if (fork_pid_child2 > 0)
                sprintf(char_pid_child2,"%d",fork_pid_child2); 
    
    if (fork_pid_child1 == 0) {                                // child process code
        argv[0] = "from_input_threads_two_childs";                         // by definition the first element must be the bin namefile
        argv[1] = firstpipe_fd0;                             // putting the char format file descriptors in the pointer to array of char (char **argv) 
        argv[2] = firstpipe_fd1;                             // by definition the last member must be NULL 
        argv[3] = secondpipe_fd0;
        argv[4] = secondpipe_fd1; 
        argv[5] = char_pid_child2;
        argv[6] = NULL;
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
    argv[1] = firstpipe_fd0;
    argv[2] = firstpipe_fd1;
        argv[3] = secondpipe_fd0;
        argv[4] = secondpipe_fd1; 
    argv[5] = NULL;
    execve("output_threads_two_childs", argv, NULL);
    perror("execve");
    }
    
    else 
    {
    wait(NULL);
    return 0;
    }     
}
