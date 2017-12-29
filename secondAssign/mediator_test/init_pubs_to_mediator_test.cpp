#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include "../SubscriptionTable.h"
#include "../auxiliary_functions.h"

using namespace std;

typedef vector<vector<int> > table_2D;
typedef vector<vector<vector<int> > > table_3D;

int main(int argc, char* argv[])
{
        const int subs_num = 3;
        const int pubs_num = 2;

        table_2D matching_table;
        table_2D pubs_fd_2Dtable;
        table_3D subs_data_fd_3Dtable;
        table_3D subs_notify_fd_3Dtable;

        char** pubs_fd_str_array;
        char** subs_data_fd_str_array;
        char** subs_notify_fd_str_array;
        char** matching_array;

        SubscriptionTable filedes_source_table(subs_num, pubs_num);

        matching_table = filedes_source_table.getTable();
        pubs_fd_2Dtable = filedes_source_table.generatePublisherPipes();
        subs_data_fd_3Dtable = filedes_source_table.generateDataPipes();
        subs_notify_fd_3Dtable = filedes_source_table.generateNotifyPipes();

        matching_array = matchingTableFrom2DVecToStrArray(matching_table);
        pubs_fd_str_array = pubsFdsFrom2DVecToStrArray(pubs_fd_2Dtable);
        subs_data_fd_str_array = subsFdsFrom3DVecToStrArray(subs_data_fd_3Dtable);
        subs_notify_fd_str_array = subsFdsFrom3DVecToStrArray(subs_notify_fd_3Dtable);

        int length_of_matching_table_str_array = 2 + subs_num * pubs_num;
        int length_of_pubs_fd_str_array = 1 + 2*pubs_num;
        int length_of_subs_fd_str_array = 2 + 2*(subs_num * pubs_num);

        pid_t* fork_pid;
        fork_pid = new pid_t[pubs_num];
        
        pid_t mediator_fork_pid = fork();
        if (mediator_fork_pid == 0)
        {
                argv[0] = "mediator_child";
                int i=1;
                int q = 0;
                while(q < length_of_subs_fd_str_array)
                {
                        argv[i] = subs_data_fd_str_array[q];
                        i++;
                        q++;  
                } 
                q = 0;
                while(q < length_of_subs_fd_str_array)
                {
                        argv[i] = subs_notify_fd_str_array[q];
                        i++;
                        q++;  
                }
                q = 0;
                while(q < length_of_pubs_fd_str_array) 
                {
                       argv[i] = pubs_fd_str_array[q];
                       i++;
                       q++;
                }
                q = 0;
                while(q < length_of_matching_table_str_array)
                {
                       argv[i] = matching_array[q];
                       i++;
                       q++; 
                }

                execve("mediator_child", argv, NULL);

        }
        else // father process
        {
                for (int i = 0; i < pubs_num; i++)
                {
                        if (i > 0)
                        {
                                if (fork_pid[i-1] != 0) // father process
                                {
                                        fork_pid[i] = fork(); // create one process with the same code that follows this point
                                }
                        }
                }

                for (int i = 0; i < pubs_num; i++)
                {
                        if(fork_pid[i] == 0) // if this process is the i-th child process
                        {
                                argv[0] = "pub_child";
                                sprintf(argv[1], "%d", i); // child process id
                                int j;
                                for (j = 2; j < length_of_pubs_fd_str_array + 2; j++) // pass the array of strings (publishers' file descriptors)
                                {
                                        argv[j] = pubs_fd_str_array[j-2];
                                }
                                argv[j+1] = NULL;
                                 
                                execve("pub_child", argv, NULL); 
                        }
                }
                for (int i = 0; i < pubs_num + 1; i++)
                {
                        wait();
                }
                return 0;
        }
}      
