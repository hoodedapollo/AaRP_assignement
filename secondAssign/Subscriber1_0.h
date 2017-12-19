#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
using namespace std;

class Subscriber {
        private:
            int subscriber_period;
            int pipe_filedes_reading;
            int pipe_filedes_writing;

        public:
            char actual_char;

            Subscriber (int);
            void set_period(int);
            void set_pipe_filedes(int, int); // set file descriptor attributes
            void set_pipe_filedes(char*, char*); // if file descriptors are chars convert them into i
            char read_from_pipe(); // read one char at a time from the pipe
};


Subscriber::Subscriber (int sub_period)
{
        subscriber_period = sub_period;
}

void Subscriber::set_period(int sub_period)
{   
        subscriber_period = sub_period;
}

void Subscriber::set_pipe_filedes( int filedes0, int filedes1)
{
        pipe_filedes_reading = filedes0;
        pipe_filedes_writing = filedes1;
        close(pipe_filedes_writing);
}

void Subscriber::set_pipe_filedes( char *filedes0, char *filedes1)
{
        pipe_filedes_reading = atoi(filedes0);
        pipe_filedes_writing = atoi(filedes1);
        close(pipe_filedes_writing);
}


char Subscriber::read_from_pipe()                                 
{
        read(pipe_filedes_reading, &actual_char, sizeof(char));
        return actual_char;
}

