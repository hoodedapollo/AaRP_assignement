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

#define PUB_NUM 2
#define SUB_TO_1 1
#define SUB_TO_2 2
#define SUB_TO_BOTH 3

class SimpleSubscriber {
        private:
            int sub_period;
            int* data_fd;
            int* notify_fd;
            int sub_notify_msg;

        public:

            SimpleSubscriber(int period, int notify_filedes[2], int data_fildes[2], int subscriptions_notify_msg);
            SimpleSubscriber(int period, char notify_filedes[2], char data_filedes[2], int subscriptions_notify_msg);
//            ~Subscriber();
            char* notify_and_read(); // notify the mediator through all the pipes and read new data if available

};

SimpleSubscriber::SimpleSubscriber(int period, int notify_filedes[2], int data_filedes[2], int subscriptions_notify_msg) // subcriptions_notify_msg: 1 --> first publisher, 2 --> second_publisher, 3 --> both (see SimpleMediator.h)
{   
        sub_period = period; // set the period with which the subscriber will send a notify to the mediator in order to get newest data 

        notify_fd = notify_filedes; 
        data_fd = data_filedes;

        close(data_fd[1]);
        close(notify_fd[0]);
        
        sub_notify_msg = subscriptions_notify_msg;
        if (sub_notify_msg != SUB_TO_1 && sub_notify_msg != SUB_TO_2 && sub_notify_msg != SUB_TO_BOTH)
        {
              cout << "\n\n ERROR: wrong subscriptions notify message number, choose among 1 or 2 or 3i\n\n";
        }
}

SimpleSubscriber::SimpleSubscriber(int period, char notify_filedes[2], char data_filedes[2], int subscriptions_notify_msg) // subcriptions_notify_msg: 1 --> first publisher, 2 --> second_publisher, 3 --> both (see SimpleMediator.h)
{   
        sub_period = period; // set the period with which the subscriber will send a notify to the mediator in order to get newest data 
        
        for (int i = 0; i < 2; i++)
        {
                notify_fd[i] = atoi(&notify_filedes[i]);
                data_fd[i] = atoi(&data_filedes[i]);
        }

        close(data_fd[1]);
        close(notify_fd[0]);
        
        sub_notify_msg = subscriptions_notify_msg;
        if (sub_notify_msg != SUB_TO_1 && sub_notify_msg != SUB_TO_2 && sub_notify_msg != SUB_TO_BOTH)
        {
              cout << "\n\n ERROR: wrong subscriptions notify message number, choose among 1 or 2 or 3i\n\n";
        }
}

// Subscriber::~Subscriber()
// {
//         delete char_read;
// }

char* SimpleSubscriber::notify_and_read() // once per period notify the mediator through a message (int msg) in all outgoing pipes and the look if the mediator has sent new data in the icoming pipes
{

        char actual_char;
        char* char_read;
        fd_set readfrom_filedes; // declare file descriptor set from which the subscriber is going to read
        FD_ZERO(&readfrom_filedes); // initialize the set
        while(1) // infinite cicle
        {
                sleep(sub_period); // do the notify and read once per period 
                        FD_SET(data_fd[0], &readfrom_filedes); // insert the incoming pipe fd in the readfrom set of file descriptors
                        write(notify_fd[1], &sub_notify_msg, sizeof(int)); // notify the mediator the subscriber wants to get new data, sub_notify_msg content also specifies from which buffers

                select(data_fd[0] + 1, &readfrom_filedes, NULL, NULL, NULL); // find if there's new data available in the data pipe
                
                        if (FD_ISSET(data_fd[0], &readfrom_filedes)) // if the data pipe has new elements
                        {
                                if (sub_notify_msg == SUB_TO_BOTH)
                                {
                                        char_read = new char[PUB_NUM];
                                        for (int i = 0; i < PUB_NUM; i++)
                                        {
                                                read(data_fd[0], &actual_char, sizeof(char)); // read a char from the pipe
                                                char_read[i] = actual_char;  // store the char in the corresponding element of the aray which will be returned
                                        }
                                }
                                else if (sub_notify_msg == SUB_TO_1 || sub_notify_msg == SUB_TO_2)
                                {
                                        char_read = new char[1];
                                        read(data_fd[0], &actual_char, sizeof(char)); // read a char from the pipe
                                        char_read[0] = actual_char;  // store the char in the corresponding element of the aray which will be returned
                                }
                               
                        }
                        return char_read;
        } 
}
