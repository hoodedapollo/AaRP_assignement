#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include "../Publisher2_0.h"

using namespace std;

int main (int argc, char* argv[])
{
        int sub_data_fd[2];
        int sub_notify_fd[2];

        char char_sub_data_fd[2][2];
        char char_sub_notify_fd[2][2];

        pipe(sub_notify_fd);
        perror("sub notify pipe");

        pipe(sub_data_fd);
        perror("sub data pipe");

        sprintf(char_sub_notify_fd[0], "%d", sub_notify_fd[0]);
        sprintf(char_sub_notify_fd[1], "%d", sub_notify_fd[1]);
        sprintf(char_sub_data_fd[0], "%d", sub_data_fd[0]);
        sprintf(char_sub_data_fd[1], "%d", sub_data_fd[1]);
        
        pid_t fork_pid = fork();
        perror("fork");
        if (fork_pid == 0)
        {
                argv[0] =(char*) "sub_child";
                argv[1] = char_sub_notify_fd[0];
                argv[2] = char_sub_notify_fd[1];
                argv[3] = char_sub_data_fd[0];
                argv[4] = char_sub_data_fd[1];
                argv[5] = NULL;

                execve(argv[0], argv, NULL);
        }
        else
        {
                Publisher pub(1, 3, sub_data_fd[0], sub_data_fd[1]);        
                pub.write_in_pipe('a');
                wait(NULL);
                return 0;
        }
}



