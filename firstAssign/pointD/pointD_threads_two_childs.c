#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
typedef void (*sighandler_t)(int);

int err_control (int func_ret, char perror_msg[], int err_value);  // error handling function

void signal_err_control(sighandler_t func_ret, char perror_msg[]);

void log_func(FILE *f);

int main (int argc, char *argv[]) {
        int fildes_firstpipe[2], fildes_secondpipe[2];  // file descriptors' arrays    
        char firstpipe_fd0[20], firstpipe_fd1[20];  // corresponding char arrays in order to pass them to the execve 
        char secondpipe_fd0[20], secondpipe_fd1[20];
        pid_t fork_pid_child1, fork_pid_child2;
        int firstpipe_ret, secondpipe_ret;  // return values
        int exec_child1_ret, exec_child2_ret;
        sighandler_t signal_sigusr1_ret, signal_sigusr2_ret;
        int first_wait_return, second_wait_return;
        FILE *f;  // declaring log file pointer
        
        f = fopen("pointD.log", "w+"); // open log file in append mode, if it does not exist create it        
        if (f == NULL)
        {
                printf("logging error occured"); fflush(stdout);
        }
        
        signal_sigusr1_ret = signal(SIGUSR1, SIG_IGN); // ignore signals coming from child1
        log_func(f);
        signal_err_control(signal_sigusr1_ret,"FATHER: signal SIGUSR1 ");
        signal_sigusr2_ret = signal(SIGUSR2,SIG_IGN);
        signal_err_control(signal_sigusr2_ret,"FATHER: signal SIGUSR1 ");

        firstpipe_ret = pipe(fildes_firstpipe);  // create the pipe, pass file descriptors to fildes
        err_control(firstpipe_ret,"first pipe",0);  // pipe1 error handling
        sprintf(firstpipe_fd0,"%d", fildes_firstpipe[0]);  // convert intger file descriptors of pipe1 to string
        sprintf(firstpipe_fd1,"%d", fildes_firstpipe[1]);                  

        secondpipe_ret = pipe(fildes_secondpipe);
        err_control(secondpipe_ret,"second pipe",0);  // pipe2 error handling
        sprintf(secondpipe_fd0,"%d", fildes_secondpipe[0]);   // convert intger file descriptors of pipe2 to string  
        sprintf(secondpipe_fd1,"%d", fildes_secondpipe[1]);           

        fork_pid_child1 = fork();  // create first child process (child1) through forking
        err_control(fork_pid_child1, "fork child 1",0);  // first fork error handling 

        if (fork_pid_child1 > 0)  // in the father procces
        {
                fork_pid_child2 = fork();  // create second child process (child2) through forking
        }

        err_control(fork_pid_child2,"fork child2", 0);  // second fork error handling

        argv[1] = firstpipe_fd0;  // put string format file descriptors in the argument vector of the execve relative to both child1 and child2 
        argv[2] = firstpipe_fd1;                             
        argv[3] = secondpipe_fd0;
        argv[4] = secondpipe_fd1; 
        argv[5] = NULL;  // by definition


        if (fork_pid_child1 == 0)  // CHILD1
        {
                argv[0] = "from_input_threads_two_childs"; // by definition the first element must be the bin path
                exec_child1_ret = execve("from_input_threads_two_childs", argv, NULL);  // execute the bin file and superimpose its process to the child1 process                  
                err_control(exec_child1_ret,"execve child1",0);  // child1 execve error handling
        }

        else if (fork_pid_child2 == 0) // CHILD2
        {
                argv[0] = "output_threads_two_childs";  // by definition the first element must be the bin path 
                exec_child2_ret = execve("output_threads_two_childs", argv, NULL);  // execute the bin file and superimpose its process to the child2 process                  ;
                err_control(exec_child2_ret,"execve child2",0);  // child2 error handling
        }

        else // FATHER
        {
                printf("child1 pid --> %d\n", fork_pid_child1); fflush(stdout);  // print child1 pid
                printf("child2 pid --> %d\n", fork_pid_child2); fflush(stdout);  // print child2 pid

                first_wait_return = wait(NULL);  // wait for a child to terminate
                err_control(first_wait_return,"first wait",0);  // first wait error handling
                if (first_wait_return == fork_pid_child2)  // if the first child to terminate is child2
                {
                        printf(" --> CHILD2 TERMINATED (PID: %d)\n", first_wait_return); fflush(stdout);  //print the pid of the first child to terminate (child2) 
                }

                second_wait_return = wait(NULL);     // wait for the other child to terminate
                err_control(second_wait_return,"second wait",0);  // second wait error handling
                if (second_wait_return == fork_pid_child1) // if the second child to terminate is child1
                {
                        printf(" --> CHILD1 TERMINATED (PID: %d)\n", second_wait_return); fflush(stdout);  //print the pid of the second child to terminate (child1) 
                }
        }     
        return 0;
}


int err_control (int func_ret, char perror_msg[], int err_value) {      // error handling function
        if (func_ret < err_value)
        {   
                perror(perror_msg);
                return -1;
        }
}

void signal_err_control(sighandler_t func_ret, char perror_msg[]) {
        if (func_ret == SIG_ERR)
        {
                perror(perror_msg);
                exit(EXIT_FAILURE);
        }
}

void log_func(FILE *f) {
    char string_pid[10];
    sprintf(string_pid, "%d", getpid());
    time_t log_time = time(NULL);
    char *string_time = asctime(localtime(&log_time));
    strtok(string_time, "\n");
    fprintf(f,"%s | PROCESS PID: %s -->  %s", string_time, string_pid,strerror(errno));
}
