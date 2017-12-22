#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

using namespace std;

int main (int argc, char* argv[])
{
        int filedes[2];
        char char_filedes[2][4];

        pipe(filedes);
        
        sprintf(char_filedes[0], "%d", filedes[0]); // convert a integer into a string of char (first row of 4 chars --> char_filedes[0] is the pointer to the first row array)
        sprintf(char_filedes[1], "%d", filedes[1]); //

        pid_t fork_pid = fork();

        if (fork_pid < 0) 
        {
                return -1;
        }

        if (fork_pid == 0) // child process
        {
                argv[0] = "publisher_test_child";
                argv[1] = char_filedes[0];        
                argv[2] = char_filedes[1];
                argv[3] = NULL;

                execve("publisher_test_child", argv, NULL);
        }

        if (fork_pid > 0) // father process
        {
                char single_char;
                int kbd_input;
                fd_set standard_input;
                FD_ZERO(&standard_input);
                close(filedes[1]);
                while(1)
                {
                        kbd_input = 0;
                        FD_SET(0, &standard_input);
                        select(1, &standard_input, NULL, NULL, NULL);
                        if (FD_ISSET(0, &standard_input))
                        {
                                read(0, &kbd_input, sizeof(int));
                                if (kbd_input == 1)
                                {
                                        return -1;
                                }
                        }        

                        read(filedes[0], &single_char, sizeof(char));
                        cout << single_char << endl;
                } 
        }

        wait(NULL);
        return 0;
}
