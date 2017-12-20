#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

using namespace std;


class Publisher {
        private:
                int pub_id;
                int publishing_period;
                int pipe_filedes_reading;
                int pipe_filedes_writing;

        public:
                Publisher(int,int); // constructor: publisher id, publisher period
                void set_publisher_id(int); // set the publisher id
                int publisher_id(); // return the publisher id
                void set_period(int); // set publishing period
                void set_pipe_filedes(int, int); // set file descriptor attributes
                void set_pipe_filedes(char*,char*); // if file des are chars convert them into int before setting the corresponding attributes
                int* publisher_filedes(); // return publisher pipe file descriptors: first is the reading one, second is the writing one
                void write_in_pipe(char); // generate a random char and write it in the pipe once per period
};

Publisher::Publisher (int id, int pub_period) // constructor: publisher id, publisher period
{
        pub_id = id;
        publishing_period = pub_period;
}

void set_publisher_id(int id) // set publisher id
{
    pub_id = id;
}

int publisher_id() // return the publisher id
{
        return pub_id;
}

void Publisher::set_period(int pub_period)
{
        publishing_period = pub_period; // seconds
}

void Publisher::set_pipe_filedes( int filedes0, int filedes1) // take the pipe reading file descriptor (fildes0) and the writing one (fileds1), sets the attributes accordingly  and closes the reading one
{
        pipe_filedes_reading = filedes0; // sets attribute accordingly
        pipe_filedes_writing = filedes1; // sets attribute accordingly
        close(pipe_filedes_reading); // closes pipe reading file descriptor
}

void Publisher::set_pipe_filedes( char *filedes0, char *filedes1) // overload in case the file descriptors are type char
{
        pipe_filedes_reading = atoi(filedes0);
        pipe_filedes_writing = atoi(filedes1);
        close(pipe_filedes_reading);
}

int* Publisher::publisher_filedes()
{       
        int fd[2];
        fd[0] = pipe_filedes_reading;
        fd[1] = pipe_filedes_writing;
        return fd;
} 

void Publisher::write_in_pipe (char Aa) // generate a random char, capitol letter or not depending on Aa, and write it in the pipe once per period
{
        char randomletter;
        while(1)
        {
                sleep(publishing_period);
                if (Aa == 'a')
                {
                        randomletter = 'a' + (random() % 26);
                }
                if (Aa == 'A')
                {
                        randomletter = 'A' + (random() % 26);
                }
                int write_return = write(pipe_filedes_writing, &randomletter, sizeof(char));
        }
}
    
