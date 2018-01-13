#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "CircularBuffer.h"

using namespace std;

#define BUFFER_SIZE 50
#define SUB_NUM 3
#define PUB_NUM 2

#define SUB_TO_1 1
#define SUB_TO_2 2
#define SUB_TO_BOTH 3
#define MED_SPACE 10
class SimpleMediator 
{
        private:
                int pubs_filedes[PUB_NUM][2]; // array of file descriptors of the same pipe: pubs_fildes[i][0] is the reading file descriptor of the i-th publisher pipe, while pubs_fildes[i][1] is the writing one 
                int subs_notify_filedes[SUB_NUM][2]; // same as pubs_fildes but for subscribers' notify pipes
                int subs_data_filedes[SUB_NUM][2]; // same as pubs_fildes but for subscribers' data pipes
                Queue buffer[PUB_NUM];
        public:
                SimpleMediator(int publisher_fd[PUB_NUM][2], int subscriber_notify_fd[SUB_NUM][2], int subscriber_data_fd[SUB_NUM][2]);
                void fromPubs_checkNotify_BufToSubs(); // check if a subscriber ha notified that it wants new data and in that case sends it the newest data sent by the publisher it is subscribed to (which is stored in a circular buffer); 
};


SimpleMediator::SimpleMediator(int publisher_fd[PUB_NUM][2], int subscriber_notify_fd[SUB_NUM][2], int subscriber_data_fd[SUB_NUM][2])
{

        //******************** set fd private variables and close file descriptors ********************************************
        for (int i = 0; i < PUB_NUM; i++)
        {
                pubs_filedes[i][0] = publisher_fd[i][0];  
                pubs_filedes[i][1] = publisher_fd[i][1];  
                close(pubs_filedes[i][1]); // close writing file descriptor of i-th publisher pipe
        }

        for (int i = 0; i < SUB_NUM; i++)
        {
                subs_notify_filedes[i][0] = subscriber_notify_fd[i][0];
                subs_notify_filedes[i][1] = subscriber_notify_fd[i][1];
                subs_data_filedes[i][0] = subscriber_data_fd[i][0];
                subs_data_filedes[i][1] = subscriber_data_fd[i][1];
                close(subs_notify_filedes[i][1]); // close writing file descriptor pf the i-th subscriber pipe
                close(subs_data_filedes[i][0]); // close the reading file descriptor of the i-th subscriber pipe
        }

        //******** initialize the buffers based on the available informations ******************************
        buffer[0].set_attributes(BUFFER_SIZE, 2); // publisher 1 is subscribed by 2 subscribers (0-th and 1-th)
        buffer[1].set_attributes(BUFFER_SIZE, 2); // publisher 2 is subscribed by 2 subscribers (              

}

void SimpleMediator::fromPubs_checkNotify_BufToSubs() // control if any publisher has written a new char in its own pipe and if that is the case add it to the correspondig buffer + check if a subscriber ha notified that it wants new data and in that case sends it the newest data sent by the publisher it is subscribed to (which is stored in a circular buffer); 
{
        char new_data;
        char buffer_data;
        int notify_msg;
        int max_pub_read_fd = 0;
        int max_sub_notify_read_fd = 0;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;

        fd_set pubs_read_fildes_set, notify_read_filedes_set; // declare set of file descriptors
        FD_ZERO(&pubs_read_fildes_set); // initialize the set of reading from publishers'pipe file descriptors
        FD_ZERO(&notify_read_filedes_set); // initialize the set of reading from subscribers' notify pipes 

        for (int i = 0; i < PUB_NUM; i++)
        {
                if (pubs_filedes[i][0] > max_pub_read_fd)
                {
                        max_pub_read_fd = pubs_filedes[i][0];
                }
        }
        cout << "MED max publisher reading fd --> " << max_pub_read_fd << endl;

        for (int i = 0; i < SUB_NUM; i++)
        {
                if (subs_notify_filedes[i][0] > max_sub_notify_read_fd)
                {
                        max_sub_notify_read_fd = subs_notify_filedes[i][0];
                }
        }
        cout << "MED max subscriber notify read fd --> " << max_sub_notify_read_fd << endl;

        while(1) 
        {
                for (int i = 0; i < PUB_NUM; i++) // assign the reading file descriptor of publisher pipes to the reading file descriptors set
                {
                        FD_SET(pubs_filedes[i][0], &pubs_read_fildes_set);
                }

                for (int i = 0; i < SUB_NUM; i++) // for all subscribers
                {
                        FD_SET(subs_notify_filedes[i][0], &notify_read_filedes_set); // add the notify reading file descriptor of the i-th subscriber to the set
                }

                int pub_sel_ret = select(max_pub_read_fd + 1, &pubs_read_fildes_set, NULL, NULL, &timeout); // check for new data in the publishers' pipes
                if (pub_sel_ret < 0)
                {
                        perror("MED select on pubs fd");
                }
                // else if (pub_sel_ret > 0)
                // {
                //         cout << string(MED_SPACE, ' ') << "MED from pub select return value --> " << pub_sel_ret << endl;
                // }

                int sub_sel_ret = select(max_sub_notify_read_fd + 1, &notify_read_filedes_set, NULL, NULL, &timeout); // check if there's new data in the subscriber's notify pipe
                if (sub_sel_ret < 0)
                {
                        perror("MED select on sub notify fd");
                }
                else if (sub_sel_ret > 0)
                {
                        cout << "MED from sub notify select return value --> " << sub_sel_ret << endl;
                }

                for (int i = 0; i < PUB_NUM; i++)  
                {
                        if ( FD_ISSET(pubs_filedes[i][0], &pubs_read_fildes_set) ) // if there is new data in the i-th publisher pipe 
                        {   
                                ssize_t from_pub_read_ret = read(pubs_filedes[i][0],&new_data,(size_t) sizeof(new_data)); // read a char from the i-th publisher pipe
                                if (from_pub_read_ret < 0)
                                {
                                        perror("MED from pub read");
                                }
                                cout << string(MED_SPACE, ' ') <<  "MED read from PUB"<<i+1<<" -->  " << new_data << endl;
                                buffer[i].enQueue(new_data); // add the char read from the i-th publisher pipe to the corresponding i-th buffer
                        }
                }

                for (int i = 0; i < SUB_NUM; i++) // for each subscriber
                {
                        if (FD_ISSET(subs_notify_filedes[i][0], &notify_read_filedes_set) ) // if a request for new data was sent through the notify pipe of the i-th subscriber 
                        {
                                int from_sub_read_ret = read(subs_notify_filedes[i][0], &notify_msg, sizeof(int)); // read the int notify_msg which is sent by the subscriber and specifies which publishers the subscriber i-th is subscribed to 
                                perror("MED from sub notify pipe read");
                                cout << "notify message --> " << notify_msg << endl;

                                if (notify_msg == SUB_TO_1 || notify_msg == SUB_TO_BOTH) // if the subscriber i-th is subscribed to topic published by the first publisher (publisher id: 0)
                                {
                                        buffer_data = buffer[0].deQueue(i); // get data from the first circular buffer (first publisher, id: 0) to be sent to the i-th subscriber 
                                        int from_buf1_write_ret =  write(subs_data_filedes[i][1], &buffer_data, sizeof(char)); // write the data in the i-th subscriber data pipe 
                                        perror("MED from buffer1 write");
                                        cout << "MED write from buffer1 to SUB" <<i+1<< " --> " << buffer_data << endl;

                                }
                                if (notify_msg == SUB_TO_2 || notify_msg == SUB_TO_BOTH)
                                {
                                        buffer_data = buffer[1].deQueue(i); // get data from the second circular buffer(second publisher, id:1) to be sent to the i-th subscriber 
                                        int from_buf2_write_ret = write(subs_data_filedes[i][1], &buffer_data, sizeof(char)); // write the data in the i-th subscriber data pipe 
                                        perror("MED from buffer2 write");
                                        cout << "MED write from buffer2 to SUB" <<i+1<< " --> " << buffer_data << endl;
                                }
                        }
                }
        }
}


