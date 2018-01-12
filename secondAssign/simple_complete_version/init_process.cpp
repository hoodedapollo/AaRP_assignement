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

        char char_pub_fd[PUBS_NUM][2][2];
        char char_sub_notify_fd[SUBS_NUM][2][2];
        char char_sub_data_fd[SUBS_NUM][2][2];

        pid_t fork_pid[SUBS_NUM + PUBS_NUM];

        for (int i = 0; i < PUBS_NUM; i++) //crate publishers' pipes and convert their fd from int to char*
        {
                cout << "\nPublisher " << i+1 << endl;
                pipe(pub_fd[i]);
                perror("pipe");
                sprintf(char_pub_fd[i][0], "%d", pub_fd[i][0]);
                cout << char_pub_fd[i][0] << endl;
                sprintf(char_pub_fd[i][1], "%d", pub_fd[i][1]);
                cout << char_pub_fd[i][1] << endl;
        }

        for (int i = 0; i < SUBS_NUM; i++) //crate subscribers' data  pipes and notify pipes convert their fd from int to char*
        {
                cout << "\nSubscriber " << i+1 << endl;
                cout << "Notify pipe" << endl;
                pipe(sub_notify_fd[i]);
                perror("pipe");
                sprintf(char_sub_notify_fd[i][0], "%d", sub_notify_fd[i][0]);
                cout << char_sub_notify_fd[i][0] << endl;
                sprintf(char_sub_notify_fd[i][1], "%d", sub_notify_fd[i][1]);
                cout << char_sub_notify_fd[i][1] << endl;
                cout << "Data pipe" << endl;    
                pipe(sub_data_fd[i]);
                perror("pipe");
                sprintf(char_sub_data_fd[i][0], "%d", sub_data_fd[i][0]);
                cout << char_sub_data_fd[i][0] << endl;
                sprintf(char_sub_data_fd[i][1], "%d", sub_data_fd[i][1]);
                cout << char_sub_data_fd[i][1] << endl;
        }

        //         pid_t mediator_fork_pid = fork();  // create the mediator child process
        //         if (mediator_fork_pid == 0)
        //         {
        //                 argv[0] = (char*) "mediator_child";
        //
        // // associate to argv all file descriptors of all pub and sub child
        //                 int q = 1;
        //                 int k = 0;
        //                 while (k < PUBS_NUM)
        //                 {
        //                         argv[q] = char_pub_fd[k][0];
        //                         q++;
        //                         argv[q] = char_pub_fd[k][1];
        //                         q++;
        //                         k++;
        //                 }
        //
        //                 while ( k < (PUBS_NUM + SUBS_NUM) )
        //                 {
        //                         argv[q] = char_sub_notify_fd[k][0];
        //                         q++;
        //                         argv[q] = char_sub_notify_fd[k][1];
        //                         q++;
        //                         argv[q] = char_sub_data_fd[k][0];
        //                         q++;
        //                         argv[q] = char_sub_data_fd[k][1];
        //                         q++;
        //                         k++;
        //                 }
        //                 cout << "MED before execve" << endl;
        //
        //                 execve("mediator_child", argv, NULL);
        //
        //         }
        //         else // father process
        //         {
        //
        // //*********** create nsubs + npubs child processes*************
        for (int i = 0; i < (PUBS_NUM + SUBS_NUM); i++)
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

        //********* exec pub child processes ********************************
        for (int i = 0; i < PUBS_NUM; i++)
        {
                if(fork_pid[i] == 0) // if this process is the i-th child process
                {
                        argv[1] = char_pub_fd[i][0];
                        argv[2] = char_pub_fd[i][1];
                        argv[3] = NULL;

                        if (i == 0)
                        {
                                argv[0] = (char*) "pub_child1";
                                cout << "PUB" << i+1 << " before execve" << endl;
                        }
                        if (i == 1)
                        {
                                argv[0] = (char*) "pub_child2";
                                cout << "PUB" << i+1 << " before execve" << endl;
                        }

                        execve(argv[0], argv, NULL);
                }
        }

        //********** exec sub child processes ********************************
        for ( int i = PUBS_NUM; i < (PUBS_NUM + SUBS_NUM); i++)
        {
                if(fork_pid[i] == 0) // if this process is the i-th child process
                {
                        argv[1] = char_sub_notify_fd[i][0];
                        argv[2] = char_sub_notify_fd[i][1];
                        argv[3] = char_sub_data_fd[i][0];
                        argv[4] = char_sub_data_fd[i][1];
                        argv[5] = NULL;


                        if (i == 2)
                        {
                                argv[0] = (char*) "sub_child1";
                                cout << "SUB" << i-1 << " before execve" << endl;
                        }
                        if (i == 3)
                        {
                                argv[0] = (char*) "sub_child2";
                                cout << "SUB" << i-1 << " before execve" << endl;
                        }
                        if (i == 4)
                        {
                                argv[0] = (char*) "sub_child3";
                                cout << "SUB" << i-1 << " before execve" << endl;
                        }

                        execve(argv[0], argv, NULL);
                }
        }
        for (int i = 0; i < (PUBS_NUM + SUBS_NUM + 1); i++) // wait for all the child processes ( + 1 stands for the mediator)
        {
                wait(NULL);
        }
        return 0;

}      
