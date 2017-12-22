#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include "auxiliary_functions.h"

using namespace std;

class Subscriber {
        private:
            int sub_id;
            int pipes_num; // it must be public for the mediator to know how many pipes it must set up for the specifc subscriber
            int sub_period;
            char* char_read; // in this case we want the information recived from the subscriber to be accessibile outside it
            int** pipe_data_filedes; // array of file descriptors of pipes from which the subscriber reads data: pipe_data_filedes[i][0] reading file descriptor of i-th pipe, pipe_data_filedes[i][1] writing file descriptor of i-th pipe
            int** pipe_notify_filedes; // array of file descriptors of pipes in which the subscriber writes a message to notify the mediator: pipe_notify_filedes[i][0] reading file descriptor of i-th pipe, pipe_notify_filedes[i][1] writing file descriptor of i-th pipe

        public:

            Subscriber(int id ,int period, int num);
            ~Subscriber();
            int get_id(); 
            int get_num_pipes(); // return the number of pipes of the subscriber
            void set_data_filedes(int**); // set file descriptor attributes of the pipe you read from
            void set_notify_filedes(int**); // set file descriptor attributes of the pipe you write into 
            char* notify_and_read(int); // notify the mediator through all the pipes and read new data if available

};

Subscriber::Subscriber(int id, int period, int num_of_pipes )
{   
        sub_id = id; 
        sub_period = period; // set the period with which the subscriber will send a notify to the mediator in order to get newest data 
        pipes_num = num_of_pipes; // set the number of data pipes (which by definition is equal to the number of notify pipes)
        char_read = new char[num_of_pipes]; // read one char per pipe 

        int_2Darray_dynamic_alloc(pipe_data_filedes, num_of_pipes, 2); // Allocate a 2D array for subscriber's data pipes file descriptors 
        int_2Darray_dynamic_alloc(pipe_notify_filedes, num_of_pipes, 2);  // Allocate a 2D array for subscriber's notify pipes file descriptors
}

Subscriber::~Subscriber()
{
        delete char_read;
        delete pipe_data_filedes;
        delete pipe_notify_filedes;
}

int Subscriber::get_id()
{
        return sub_id;
}

int Subscriber::get_num_pipes() // return the number of pipes of the subscriber
{
        return pipes_num;
}

void Subscriber::set_data_filedes( int** filedes) // takes an array of bidimensional arrays which element are the reading file descriptors and the writing ones of the i-th data pipe
{
        for (int i = 0; i < pipes_num; i++) // for each pipe 
        {
                pipe_data_filedes[i][0] = filedes[i][0]; // set pipe_data_filedes[i][0] as the reading file descriptor of the i-th data pipe
                pipe_data_filedes[i][1] = filedes[i][1]; // set pipe_data_filedes[i][1] as the writing file descriptor of the i-th data pipe 
                close(pipe_data_filedes[i][1]); // close all writing file descriptors of the data pipes 
        }
}

void Subscriber::set_notify_filedes( int** filedes) // takes an array of bidimensional arrays which element are the reading file descriptors and the writing ones of the i-th notify pipe
{
        for (int i = 0; i < pipes_num; i++) // for each pipe 
        {
                pipe_notify_filedes[i][0] = filedes[i][0]; // set pipe_data_filedes[i][0] as the reading file descriptor of the i-th notify pipe
                pipe_notify_filedes[i][1] = filedes[i][1]; // set pipe_data_filedes[i][1] as the writing file descriptor of the i-th notify pipe 
                close(pipe_notify_filedes[i][0]); // close all reading file descriptors of the notify pipes 
        }
}

char* Subscriber::notify_and_read(int msg) // once per period notify the mediator through a message (int msg) in all outgoing pipes and the look if the mediator has sent new data in the icoming pipes
{

        char actual_char;
        fd_set readfrom_fildes; // declare file descriptor set from which the subscriber is going to read
        FD_ZERO(&readfrom_fildes); // initialize the set
        while(1) // infinite cicle
        {
                sleep(sub_period); // do the notify and read once per period 
                for (int i = 0; i < pipes_num; i++) // for each pipe
                {
                        FD_SET(pipe_data_filedes[i][0], &readfrom_fildes); // insert the incoming pipes file descriptors in the readfrom set of file descriptors
                        write(pipe_notify_filedes[i][1], &msg, sizeof(int)); // send an int type notify writing it the outgoing pipes
                }

                int max_fd = max_positve_element(pipe_data_filedes[0]); // compute the maximum file descriptor among the ones in the readfrom set
                select(max_fd + 1, &readfrom_fildes, NULL, NULL, NULL); // find which pipes among the readfrom set have new elemnts in them
                
                for (int i = 0; i < pipes_num; i++) // for each pipe
                {
                        if (FD_ISSET(pipe_data_filedes[i][0], &readfrom_fildes)) // if the incoming pipe has new elements
                        {
                                read(pipe_data_filedes[i][0], &actual_char, sizeof(char)); // read a char from the pipe
                                char_read[i] = actual_char;  // store the char in the corresponding element of the aray which will be returned
                        }
                        else
                        {   
                                char_read[i] = 0; // if there are no new elements in the pipe store a NULL
                        }
                }

                return char_read; // return the array 
        }
} 

