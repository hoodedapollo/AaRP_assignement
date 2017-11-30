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

extern int err_control(int func_ret, char perror_msg[], int err_value) {      // error handling function
        if (func_ret < err_value)
        {
                perror(perror_msg);
                return -1;
        }
}

extern void signal_err_control(sighandler_t func_ret, char perror_msg[]) {
        if (func_ret == SIG_ERR)
        {
                perror(perror_msg);
                exit(EXIT_FAILURE);
        }
}
extern void log_func(char msg[]) {
    FILE *f = fopen("pointrC.log","a+");  // open the log file in append mode
    char string_pid[5];
    sprintf(string_pid, "%d", getpid()); // trasform int pid into a character array 
    time_t log_time = time(NULL);  // get time, day, month ...
    char *string_time = asctime(localtime(&log_time));// convert time_t log_time into a string
    strtok(string_time, "\n"); // remove the last element of the array which is \n  
    fprintf(f,"%s | PROCESS PID: %s --> %s:  %s\n", string_time, string_pid, msg, strerror(errno)); // w
    fclose(f); // close the log file
}

void handler(int signo) {  // signal handling function
        if (signo == SIGUSR1)
        {
                printf("CHILD1: Signal %d recieved. Terminating child1 process", signo); fflush(stdout);
                exit(EXIT_SUCCESS);
        }
        if (signo == SIGUSR2)
        {
                printf("CHILD2: Signal %d recieved. Terminating child2 process", signo);
                exit(0);
        }

}

void vector_print(int a[],char msg[]) {  // printing vector function
        printf("\n%s\n[", msg);

        for (int i=0;i<10;i++)
        {
                printf(" %d", a[i]);
        }
        printf(" ]\n\n");
}

void bubble_sort(int *a) {  // vector sorting function
        for(int j = 0; j<10; j++)
        {
                for(int i=0;i<9;i++)
                {
                        if (a[i] > a[i+1])
                        {
                                int temp = a[i];
                                a[i] = a[i+1];
                                a[i+1] = temp;
                        }
                }
        }
}


