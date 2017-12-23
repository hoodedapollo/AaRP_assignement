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

        sprintf(char_filedes[0], "%d", filedes[0]); // convert a integer into a string of char (first row of 4 chars --> char_filedes[0] is the pointer to the first row array)
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
                argv[0] = "publisher_test_child";
                argv[1] = char_filedes[0];        
                argv[2] = char_filedes[1];
                argv[3] = NULL;
                
                cout << "CHILD before execve\n" << endl;
//                while(1){;
                execve("publisher_test_child", argv, NULL);
        }

        else // father process
        {
                char single_char;
//                int kbd_input;
//                fd_set standard_input;
//                FD_ZERO(&standard_input);
                close(filedes[1]);
                while(1)
                {
//                        kbd_input = 0;
//                        FD_SET(0, &standard_input);
//                        select(1, &standard_input, NULL, NULL, NULL);
//                        if (FD_ISSET(0, &standard_input))
//                        {
//                                read(0, &kbd_input, sizeof(int));
//                                if (kbd_input == 1)
//                                {
//                                        return -1;
//                                }
//                        }        

                        read(filedes[0], &single_char, sizeof(single_char));
                        perror("read");
                        cout << "FATHER: read " << single_char << endl;
                } 
                int wait_ret = wait(NULL);
                cout << "wait return --> " << wait_ret << endl;
        }

        return 0;
}
