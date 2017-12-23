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
        int filedes[2];
        char char_filedes[2][6];

        pipe(filedes);
        perror("pipe");

        cout << "FATHER: fildes[0] --> " << filedes[0] << endl;
        cout << "FATHER: fildes[1] --> " << filedes[1] << endl;

        sprintf(char_filedes[0], "%d", filedes[0]); // convert a integer into a string of char (first ro
        sprintf(char_filedes[1], "%d", filedes[1]); //

        cout << "FATHER: char_fildes[0] --> " << char_filedes[0] << endl;
        cout << "FATHER: char_fildes[1] --> " << char_filedes[1] << endl;

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
                argv[1] = char_filedes[0];
                argv[2] = char_filedes[1];
                argv[3] = NULL;

                execve("subscriber_test_child", argv, NULL);
        }
        else // father process
        {
                char single_char = 'A';
                fd_set notify_input_set;
                FD_ZERO(&notify_input_set);
                while(1)
                {
                        FD_SET(filedes[0], &notify_input_set);
                        select(filedes[1] + 1, &notify_input_set, NULL, NULL, NULL)
                        if (FD_ISSET(filedes[0], &notify_input_set))
                        {
                                write(filedes[1], &single_char, sizeof(single_char));
                        }
                }
                int wait_ret = wait(NULL);
                cout << "FATHER: wait return --> " << wait_ret << endl;
        }

        return 0;
}

