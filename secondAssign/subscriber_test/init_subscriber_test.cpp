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


int main (int argc, char* argv[])
{
        int pipes_num = 2;
        vector<int> row_fd(2,0);
        vector<vector<int> > vector_filedes(pipes_num,row_fd); // fildes[2][2]
        char char_filedes[2][2][6];

        for (int i = 0; i < pipes_num; i++) // for each row which denotes a pipe
        {
                pipe(vector_filedes[i].data());              
                perror("pipe");
        }

        cout << "FATHER:" << endl;
        for (unsigned int i = 0; i < vector_filedes.size(); i++)
        {
                for (int j = 0; j < 2; j++)
                {
                        cout << "first pipe reading int fd: vector_filedes[" << i << "][" << j << "] --> " << vector_filedes[i][j] << endl;
                }
        }

        for (unsigned int i = 0; i < vector_filedes.size(); i++)
        {
                for (int j = 0; j < 2; j++)
                {
                    sprintf(char_filedes[i][j], "%d", vector_filedes[i][j]); // convert first pipe reading integer file descriptor into a string of char 
                }
        }

        cout << "FATHER:" << endl;
        cout << "first pipe reading char fd: char_filedes[0][0] --> " << char_filedes[0][0] << endl;
        cout << "first pipe writing char fd: char_filedes[0][1] --> " << char_filedes[0][1] << endl;

        cout << "second pipe reading char fd: char_fildes[1][0] --> " << char_filedes[1][0] << endl;
        cout << "second pipe writing char fd: char_fildes[1][1] --> " << char_filedes[1][1] << endl;

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
                int q = 0;
                while (j < pipes_num)
                {
                        argv[q] = char_filedes[j][0]; // reading file descriptor of the j-th pipe
                        q++; // increase the argv index
                        argv[q] = char_filedes[j][1]; // writing file descritpor of the j-th pipe
                        q++; // increase the argv index
                        j++; // increase the pipe index
                }
                argv[q+1] = NULL; // the last elment of the argv array must be NULL 

                execve("subscriber_test_child", argv, NULL);
        }
        else // father process
        {
                char single_char = 'A';
                fd_set notify_input_set; // declare the reading file descriptors set of all the notify pipes
                FD_ZERO(&notify_input_set);
                while(1)
                {
                        for (int i = 0; i < pipes_num; i++) // for all pipes
                        {
                                FD_SET(vector_filedes[i][0], &notify_input_set); // each loop add the reading file descriptor of the i-th notify pipe 
                        }

                        select(max_positive_in_column_2D_array(vector_filedes,0) + 1, &notify_input_set, NULL, NULL, NULL); // check if there is some new data in all the notify pipes, if not remove the corresponding reading file descriptor from the notify_input_set

                        for (int i = 0; i < pipes_num; i++)
                        {        
                                if (FD_ISSET(vector_filedes[i][0], &notify_input_set)) // check if the reading file descriptor of the i-th notify pipe is still in the notify_input_set
                                {
                                        write(vector_filedes[i][1], &single_char, sizeof(single_char)); // write the content of single_char in the i-th data pipe 
                                }
                        }
                }
                int wait_ret = wait(NULL);
                cout << "FATHER: wait return --> " << wait_ret << endl;
        }

        return 0;
}

