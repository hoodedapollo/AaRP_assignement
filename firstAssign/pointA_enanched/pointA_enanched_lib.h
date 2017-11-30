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

extern int err_control(int func_ret, char perror_msg[], int err_value)       // error handling function
{
        if (func_ret < err_value)
        {
                perror(perror_msg);
                return -1;
        }
}

extern void signal_err_control(sighandler_t func_ret, char perror_msg[])
{
        if (func_ret == SIG_ERR)
        {
                perror(perror_msg);
                exit(EXIT_FAILURE);
        }
}

extern void log_func(char msg[])
{
    FILE *f = fopen("pointA_enanched.log","a+");
    char string_pid[5];
    sprintf(string_pid, "%d", getpid());
    time_t log_time = time(NULL);
    char *string_time = asctime(localtime(&log_time));
    strtok(string_time, "\n");
    fprintf(f,"%s | PROCESS PID: %s --> %s:  %s\n", string_time, string_pid, msg, strerror(errno));
    fclose(f);
}


