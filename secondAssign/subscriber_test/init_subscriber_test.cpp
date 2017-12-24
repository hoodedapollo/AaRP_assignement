#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;


int main (int argc, char* argv[])
{
        int pipes_num = 2;
        int filedes[2][2];
        char char_filedes[2][2][6];

        for (int i = 0; i < pipes_num; i++)
        {
                pipe(filedes[i]);
                perror("pipe");
        }

        cout << "FATHER:" << endl;
        cout << "first pipe reading int fd: fildes[0][0] --> " << filedes[0][0] << endl;
        cout << "first pipe writing int fd: fildes[0][1] --> " << filedes[0][1] << endl;

        cout << "second pipe reading int fd: fildes[1][0] --> " << filedes[1][0] << endl;
        cout << "second pipe writing int fd: fildes[1][1] --> " << filedes[1][1] << endl;

        sprintf(char_filedes[0][0], "%d", filedes[0]); // convert first pipe reading integer file descriptor into a string of char 
        sprintf(char_filedes[0][1], "%d", filedes[1]); // convert first pipe writing integer file descriptor into a string of char  

        sprintf(char_filedes[1][0], "%d", filedes[0]); // convert second pipe reading integer file descriptor into a string of char 
        sprintf(char_filedes[1][1], "%d", filedes[1]); // convert second pipe writing integer file descriptor into a string of char  

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
                        argv[q] = char_filedes[j][0];
                        q++;
                        argv[q] = char_filedes[j][1];
                        q++;
                        j++;
                }
                argv[q+1] = NULL;

                execve("subscriber_test_child", argv, NULL);
        }
        else // father process
        {
                char single_char = 'A';
                fd_set notify_input_set;
                FD_ZERO(&notify_input_set);
                while(1)
                {
                        for (int i = 0; i < pipes_num; i++)
                        {
                                FD_SET(filedes[i][0], &notifyi_input_set);
                        }

                        select(filedes[PIPES_NUM-1][0] + 1, &notify_input_set, NULL, NULL, NULL);

                        for (int i = 0; i PIPES_NUM; ++)
                        {        
                                if (FD_ISSET(filedes[i][0], &notify_input_set))
                                {
                                        write(filedes[i][1], &single_char, sizeof(single_char));
                                }
                        }
                }
                int wait_ret = wait(NULL);
                cout << "FATHER: wait return --> " << wait_ret << endl;
        }

        return 0;
}

