#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include "auxiliary_functions.h"
#include <vector>
#include <stdio.h>
using namespace std;

class Subscriber {
        private:
            int pipes_num; // it must be public for the mediator to know how many pipes it must set up for the specifc subscriber
            int sub_period;
            vector<char> char_read; // in this case we want the information recived from the subscriber to be accessibile outside it
            vector<vector<int> > vector_data_filedes;
            vector<vector<int> > vector_notify_filedes;


        public:

            Subscriber(int period, int num_of_pipes, vector<vector<int> > data_filedes, vector<vector<int> > notify_filedes);
//            ~Subscriber();
            int get_num_pipes(); // return the number of pipes of the subscriber
            void notify_and_read(int msg); // notify the mediator through all the pipes and read new data if available

};

Subscriber::Subscriber(int period, int num_of_pipes, vector<vector<int> > data_filedes, vector<vector<int> > notify_filedes )
{   
        sub_period = period; // set the period with which the subscriber will send a notify to the mediator in order to get newest data 
        pipes_num = num_of_pipes; // set the number of data pipes (which by definition is equal to the number of notify pipes)
        char_read.resize(num_of_pipes); // read one char per pipe 

        vector_data_filedes = data_filedes;
        vector_notify_filedes = notify_filedes; 

        for (unsigned int i = 0; i < vector_data_filedes.size(); i++)
        {
                close(vector_data_filedes[i][1]);
                close(vector_notify_filedes[i][0]);
        }
}

// Subscriber::~Subscriber()
// {
//         delete char_read;
// }

int Subscriber::get_num_pipes() // return the number of pipes of the subscriber
{
        return pipes_num;
}

void Subscriber::notify_and_read(int msg) // once per period notify the mediator through a message (int msg) in all outgoing pipes and the look if the mediator has sent new data in the icoming pipes
{

        char actual_char;
        fd_set readfrom_filedes; // declare file descriptor set from which the subscriber is going to read
        FD_ZERO(&readfrom_filedes); // initialize the set
        while(1) // infinite cicle
        {
                sleep(sub_period); // do the notify and read once per period 
                for (int i = 0; i < pipes_num; i++) // for each pipe
                {
                        FD_SET(vector_data_filedes[i][0], &readfrom_filedes); // insert the incoming pipes file descriptors in the readfrom set of file descriptors
                        write(vector_notify_filedes[i][1], &msg, sizeof(msg)); // send an int type notify writing it the outgoing pipes
                }

                int max_fd = max_positive_in_column_2D_array(vector_data_filedes,0); // compute the maximum file descriptor among the ones in the readfrom set
                select(max_fd + 1, &readfrom_filedes, NULL, NULL, NULL); // find which pipes among the readfrom set have new elemnts in them
                
                for (int i = 0; i < pipes_num; i++) // for each pipe
                {
                        if (FD_ISSET(vector_data_filedes[i][0], &readfrom_filedes)) // if the incoming pipe has new elements
                        {
                                read(vector_data_filedes[i][0], &actual_char, sizeof(char)); // read a char from the pipe
                                char_read[i] = actual_char;  // store the char in the corresponding element of the aray which will be returned
                        }
                        else
                        {   
                                char_read[i] = '-'; // if there are no new elements in the pipe store a NULL
                        }
                        cout << "char_read[" << i << "] -->" << char_read[i] << endl;
                }
        }
} 

