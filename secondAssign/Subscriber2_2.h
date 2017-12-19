#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>

using namespace std;

class Subscriber {
        private:
            int subscriber_period;
            int *pipe_filedes_reading_open; 
            int *pipe_filedes_writing_close;
            int *pipe_filedes_reading_close;
            int *pipe_filedes_writing_open;        
            int num_pipe;

        public:
            char actual_char;

            Subscriber (int);
            void set_period(int);
            void set_num_pipe(int);
            void set_readfrompipe_filedes(int*, int*); // set file descriptor attributes of the pipe you read from
            void set_writeintopipe_filedes(int*, int*); // set file descriptor attributes of the pipe you write into 
            char notify_and_read(int);
};

void Subscriber::set_period(int sub_period)
{   
        subscriber_period = sub_period;
}

void Subscriber::set_num_pipe(int number)
{
        num_pipe = number;
        pipe_filedes_reading_open = new int [num_pipe];
        pipe_filedes_writing_close = new int [num_pipe];
        pipe_filedes_reading_close = new int [num_pipe];
        pipe_filedes_writing_open = new int [num_pipe];
}

void Subscriber::set_readfrompipe_filedes( int *filedes0, int *filedes1)
{
        for (int i = 0; i < num_pipe; i++)
        {
                pipe_filedes_reading_open[i] = filedes0[i];
                pipe_filedes_writing_close[i] = filedes1[i];
                close(pipe_filedes_writing_close[i]);
        }
}

void Subscriber::set_writeintopipe_filedes( int *filedes0, int *filedes1)
{
        for (int i = 0; i < num_pipe; i++)
        {
                pipe_filedes_reading_close[i] = filedes0[i];
                pipe_filedes_writing_open[i] = filedes1[i];
                close(pipe_filedes_reading_close[i]);
        }
}

char Subscriber::notify_and_read(int msg)
{
        fd_set readfrom_fildes;
        FD_ZERO(&readfrom_fildes);
        while(1)
        {
                sleep(subscriber_period);
                for (int which_pipe = 0; which_pipe < num_pipe; which_pipe++)
                {
                        write(pipe_filedes_writing_open[which_pipe], &msg, sizeof(int));
                        FD_SET(pipe_filedes_reading_open[which_pipe], &readfrom_fildes);
                        select(pipe_filedes_reading_open[which_pipe] + 1, &readfrom_fildes, NULL, NULL, NULL);
                        if (FD_ISSET(pipe_filedes_reading_open[which_pipe], &readfrom_fildes))
                        {
                                read(pipe_filedes_reading_open[which_pipe], &actual_char, sizeof(char));
                        }
                }
                return actual_char;
        }
} 
