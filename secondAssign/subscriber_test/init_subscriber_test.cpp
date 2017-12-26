#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/wait.h>
#include "../auxiliary_functions.h"
#include <vector>

using namespace std;

#define PIPES_NUM 2

int main (int argc, char* argv[])
{
        int pipes_num = PIPES_NUM;
        vector<int> row_fd(2,0);
        vector<vector<int> > vector_data_filedes(pipes_num,row_fd); // fildes[2][2]
        vector<vector<int> > vector_notify_filedes(pipes_num,row_fd); // fildes[2][2]
        char char_data_filedes[2][2][6];
        char char_notify_filedes[2][2][6];

        for (int i = 0; i < pipes_num; i++) // for each row which denotes a pipe
        {
                pipe(vector_data_filedes[i].data());              
                perror("pipe");
                pipe(vector_notify_filedes[i].data());              
                perror("pipe");
        }

        cout << "FATHER:" << endl;
        for (int i = 0; i < pipes_num; i++)
        {
                for (int j = 0; j < 2; j++)
                {
                        cout << "data pipe int fd: vector_data_filedes[" << i << "][" << j << "] --> " << vector_data_filedes[i][j] << endl;
                        sprintf(char_data_filedes[i][j], "%d", vector_data_filedes[i][j]); // convert first pipe reading integer file descriptor into a string of char 
                        cout << "data pipe char fd: char_data_filedes[" << i << "][" << j << "] --> " << char_data_filedes[i][j] << endl << endl;

                        cout << "notify pipe int fd: vector_notify_filedes[" << i << "][" << j << "] --> " << vector_notify_filedes[i][j] << endl;
                        sprintf(char_notify_filedes[i][j], "%d", vector_notify_filedes[i][j]); // convert first pipe reading integer file descriptor into a string of char 
                        cout << "notify pipe char fd: char_notify_filedes[" << i << "][" << j << "] --> " << char_notify_filedes[i][j] << endl << endl;
                }
        }

        pid_t fork_pid = fork();
        perror("fork");
        cout << "fork_pid --> " << fork_pid << endl;

        if (fork_pid == -1)
        {
                return -1;
        }

        else if (fork_pid == 0) // child process
        {
                argv[0] = "subscriber_test_child";

                int j = 0;
                int q = 1;
                while (j < pipes_num)
                {
                        argv[q] = char_data_filedes[j][0]; // reading file descriptor of the j-th pipe
                        q++; // increase the argv index
                        argv[q] = char_data_filedes[j][1]; // writing file descritpor of the j-th pipe
                        q++; // increase the argv index
                        j++; // increase the pipe index
                }
                j = 0;
                while (j < pipes_num)
                {
                        argv[q] = char_notify_filedes[j][0]; // reading file descriptor of the j-th pipe
                        q++; // increase the argv index
                        argv[q] = char_notify_filedes[j][1]; // writing file descritpor of the j-th pipe
                        q++; // increase the argv index
                        j++; // increase the pipe index
                }

                argv[q] = NULL; // the last elment of the argv array must be NULL

                execve("subscriber_test_child", argv, NULL);
        }
        else // father process
        {
                char single_char;
                fd_set notify_input_set; // declare the reading file descriptors set of all the notify pipes
                FD_ZERO(&notify_input_set);
                while(1)
                {

                        for (int i = 0; i < pipes_num; i++) // for all pipes
                        {
                                FD_SET(vector_notify_filedes[i][0], &notify_input_set); // each loop add the reading file descriptor of the i-th notify pipe
                        }

                        select(max_positive_in_column_2D_array(vector_notify_filedes,0) + 1, &notify_input_set, NULL, NULL, NULL); // check if there is some new data in all the notify pipes, if not remove the corresponding reading file descriptor from the notify_input_set

                        for (int i = 0; i < pipes_num; i++)
                        {
                                single_char = 'A' + (rand() % 26);

                                if (FD_ISSET(vector_notify_filedes[i][0], &notify_input_set)) // check if the reading file descriptor of the i-th notify pipe is still in the notify_input_set
                                {
                                        write(vector_data_filedes[i][1], &single_char, sizeof(single_char)); // write the content of single_char in the i-th data pipe
                                }
                        }
                }
                int wait_ret = wait(NULL);
                cout << "FATHER: wait return --> " << wait_ret << endl;
        }

        return 0;
}

