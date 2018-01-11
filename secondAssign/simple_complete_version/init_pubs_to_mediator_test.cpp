#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

#define SUBS_NUM 3
#define PUBS_NUM 2

int main(int argc, char* argv[])
{
        
        int pub_fd[PUBS_NUM][2];
        int sub_notify_fd[SUBS_NUM][2];
        int sub_data_fd[SUBS_NUM][2];

        char* char_pub_fd[PUBS_NUM][2];
        char* char_sub_notify_fd[SUBS_NUM][2];
        char* char_sub_data_fd[SUBS_NUM][2];

        pid_t fork_pid[SUBS_NUM + PUBS_NUM];

        for (int i = 0; i < PUBS_NUM; i++) //crate publishers' pipes and convert their fd from int to char*
        {
                pipe(pub_fd[i]);
                sprintf(char_pub_fd[i][0], "%d", pub_fd[i][0]);
                sprintf(char_pub_fd[i][1], "%d", pub_fd[i][1]);
        }

        for (int i = PUBS_NUM; i < (PUBS_NUM + SUBS_NUM); i++) //crate subscribers' data  pipes and notify pipes convert their fd from int to char*
        {
                pipe(sub_notify_fd[i]);
                sprintf(char_sub_notify_fd[i][0], "%d", sub_notify_fd[i][0]);
                sprintf(char_sub_notify_fd[i][1], "%d", sub_notify_fd[i][1]);

                pipe(sub_data_fd[i]);
                sprintf(char_sub_data_fd[i][0], "%d", sub_data_fd[i][0]);
                sprintf(char_sub_data_fd[i][1], "%d", sub_data_fd[i][1]);
        }

        
        pid_t mediator_fork_pid = fork();  // create the mediator child process
        if (mediator_fork_pid == 0)
        {
                argv[0] = "mediator_child";

//*** associate to argv all file descriptors of all pub and sub child ***
                int q = 0;
                int k = 0;

                while(k < PUBS_NUM)
                {
                        argv[q] = char_pub_fd[k][0];
                        q++;
                        argv[q] = char_pub_fd[k][1];
                        q++;
                        k++;
                }

                while (k < SUBS_NUM)
                {
                        argv[q] = char_sub_notify_fd[k][0];
                        q++;
                        argv[q] = char_sub_notify_fd[k][1];
                        q++;
                        argv[q] = char_sub_data_fd[k][0];
                        q++;
                        argv[q] = char_sub_data_fd[k][1];
                        q++;
                        k++;
                }
//**************************************************************************

                arg[k] = NULL;

                execve("mediator_child", argv, NULL);

        }
        else // father process
        {

//************ create nsubs + npubs child processes*************
                for (int i = 0; i < (PUBS_NUM + PUBS_NUM); i++)
                {
                        if (i == 0) // first time just fork
                        {
                                fork_pid[i] = fork();
                        }
                        else // after the first fork if father process fork, if child process exit the for loop
                        {
                                if (fork_pid[i-1] != 0) // father process
                                {
                                        fork_pid[i] = fork(); // create one process with the same code that follows this point
                                }
                                else // child process
                                {
                                        break;
                                }
                        }
                }

//********** exec pub child processes ********************************
                for (int i = 0; i < PUBS_NUM; i++)
                {
                        if(fork_pid[i] == 0) // if this process is the i-th child process
                        {
                                argv[1] = char_pub_fd[i][0];
                                argv[2] = char_pub_fd[i][1];
                                argv[3] = NULL;

                                if (i == 0)
                                {
                                        argv[0] = "pub_child1";
                                }
                                if (i == 1) 
                                {
                                        argv[0] = "pub_child2";
                                }
                                 
                                execve(argv[0], argv, NULL); 
                        }
                }

//*********** exec sub child processes ********************************
                for ( int i = PUBS_NUM; i < (PUBS_NUM + SUBS_NUM); i++)
                {
                       if(fork_pid[i] == 0) // if this process is the i-th child process 
                       {
                               argv[1] = char_sub_notify_fd[i][0];
                               argv[2] = char_sub_notify_fd[i][1];
                               argv[3] = char_sub_data_fd[i][0];
                               argv[4] = char_sub_data_fd[i][1]; 
                               argv[5] = NULL;
                       }

                       if (i == 2)
                       {
                               argv[0] = "sub_child1";
                       }
                       if (i == 3)
                       {
                               argv[0] = "sub_child2";
                       }
                       if (i == 4)
                       {
                               argv[0] = "sub_child3";
                       }

                       execve(argv[0], argv, NULL);
                }
//*** wait for all the child processes ( + 1 stands for the mediator) ***
                for (int i = 0; i < (PUBS_NUM + SUBS_NUM + 1); i++) 
                {
                        wait(NULL);
                }
                return 0;
        }
}      
