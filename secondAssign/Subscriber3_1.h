#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <algorithm>

using namespace std;

int max_positve_element (int *array)  // maximus positive element in an integer vector
{
        max_fd = 0;
        int size = sizeof(array)/sizeof(int)
        for (int i=0; i < size; i++)
        {
                if (array[i] > max_fd)
                {
                        max_fd = array[i];
                } 
        return max_fd;
}

class Subscriber {
        private:
            int num_pipe; // it must be public for the mediator to know how many pipes it must set up for the specifc subscriber
            int subscriber_period;
            char* actual_char; // in this case we want the information recived from the subscriber to be accessibile outside it
            int *pipe_filedes_reading_open; // reading file descriptors of incoming pipes which must remain open
            int *pipe_filedes_writing_close; // writing file descriptors of incoming pipes which must be closed
            int *pipe_filedes_reading_close; // reading file descriptors of outgoing pipes which must be closed
            int *pipe_filedes_writing_open;  // writing file descriptors of outgoing pipes  which must remain open

        public:

            Subscriber (int);
            void set_period(int); // set the period with which the subscriber will send a notify to the mediator in order to get newest data 
            void set_num_pipe(int); // set the number of pipes
            int how_many_pipes(); // return the number of pipes of the subscriber
            void set_readfrompipe_filedes(int*, int*); // set file descriptor attributes of the pipe you read from
            void set_writeintopipe_filedes(int*, int*); // set file descriptor attributes of the pipe you write into 
            char notify_and_read(int); // notify the mediator through all the pipes and read new data if available

};

void Subscriber::set_period(int sub_period)
{   
        subscriber_period = sub_period;
}

void Subscriber::set_num_pipe(int number) // set two pipe for each notify and read you need to perform since unidirectional pipes are needed 
{
        num_pipe = number;

        // readfrom pipes
        pipe_filedes_reading_open = new int [num_pipe];
        pipe_filedes_writing_close = new int [num_pipe];
        
        // notify pipes
        pipe_filedes_reading_close = new int [num_pipe];
        pipe_filedes_writing_open = new int [num_pipe];
}

int Subscriber::how_many_pipes() // return the number of pipes of the subscriber
{
        return num_pipe;
}

void Subscriber::set_readfrompipe_filedes( int *filedes0, int *filedes1) // takes reading (fildes0) and writing (fildes1) file descriptors of READFROM pipes and sets attributes accordingly
{
        for (int which_pipe = 0; which_pipe < num_pipe; which_pipe++) // for each pipe 
        {
                pipe_filedes_reading_open[which_pipe] = filedes0[which_pipe]; // set relative subscriber attributes accordingly
                pipe_filedes_writing_close[which_pipe] = filedes1[which_pipe]; // set relative subscriber attribute accordingly
                close(pipe_filedes_writing_close[which_pipe]); // close incoming pipe writing file descriptor 
        }
}

void Subscriber::set_writeintopipe_filedes( int *filedes0, int *filedes1) // takes reading (fildes0) and writing (fildes1) file descriptors of notify (WRITE INTO) pipes and sets attributes accordingly
{
        for (int which_pipe = 0; which_pipe < num_pipe; which_pipe++)
        {
                pipe_filedes_reading_close[which_pipe] = filedes0[which_pipe]; // set relative subscriber attributes accordingly
                pipe_filedes_writing_open[which_pipe] = filedes1[which_pipe]; // set relative subscriber attributes accordingly
                close(pipe_filedes_reading_close[which_pipe]); // close outgoing pipe reading file descriptor 
        }
}

char* Subscriber::notify_and_read(int msg) // once per period notify the mediator through a message (int msg) in all outgoing pipes and the look if the mediator has sent new data in the icoming pipes
{

        fd_set readfrom_fildes; // declare file descriptor set from which the subscriber is going to read
        FD_ZERO(&readfrom_fildes); // initialize the set
        while(1) // infinite cicle
        {
                sleep(subscriber_period); // do the notify and read once per period 
                for (int which_pipe = 0; which_pipe < num_pipe; which_pipe++) // for each pipe
                {
                        FD_SET(pipe_filedes_reading_open[which_pipe], &readfrom_fildes); // insert the incoming pipes fd in the readfrom set of file descriptors
                        write(pipe_filedes_writing_open[which_pipe], &msg, sizeof(int)); // send a notify through a int writing it the outgoing pipes
                }

                int max_fd = max_positve_element(pipe_filedes_reading_open) // compute the maximum file descriptor among the ones in the readfrom set
                select(max_fd + 1, &readfrom_fildes, NULL, NULL, NULL); // find which pipes among the readfrom set have new elemnts in them
                
                int i = 0;
                char char_read[num_pipe]; // declare a char array with as many elements as the number of the incoming pipes 
                for (int which_pipe = 0; which_pipe < num_pipe; which_pipe++) // for each pipe
                {
                        if (FD_ISSET(pipe_filedes_reading_open[which_pipe], &readfrom_fildes)) // if the incoming pipe has new elements
                        {
                                read(pipe_filedes_reading_open[which_pipe], &actual_char, sizeof(char)); // read a char from the pipe
                                char_read[which_pipe] = actual_char;  // store the char in the corresponding element of the aray which will be returned
                        }
                        else
                        {   
                                char_read[which_pipe] = NULL; // if there are no new elements in the pipe store a NULL
                        }
                }

                return char_read; // return the array 
        }
} 
