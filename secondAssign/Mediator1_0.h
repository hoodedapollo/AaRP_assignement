#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include "CircularBuffer.h"
#include "auxiliary_functions.h"

using namespace std;

#define BUFFER_SIZE 50

class Mediator 
{
        private:
                int pubs_num; // number of publishers which is equal to the number of buffers they respectively write into
                int subs_num; // number of subscribers
                int** subs_to_pubs_matching_table; // bidimensional array containing the subs id and the id of the publishers it is subscribed to. 
                int** pubs_filedes; // array of file descriptors of the same pipe: pubs_fildes[i][0] is the reading file descriptor of the i-th publisher pipe, while pubs_fildes[i][1] is the writing one 
                int** subs_notify_filedes; // same as *pubs_fildes but for subscribers' notify pipes
                int** subs_data_filedes; // same as *pubs_fildes but for subscribers' readfrom pipes
                Queue* buffer;
        public:
                Mediator(int num_of_pubs, int** pubslisher_fd, int num_of_subs, int** subscriber_data_fd, int** subscriber_notify_fd, int** matching_table);
                ~Mediator();
                void fromPubs_checkNotify_BufToSubs(); // check if a subscriber ha notified that it wants new data and in that case sends it the newest data sent by the publisher it is subscribed to (which is stored in a circular buffer); 
};


Mediator::Mediator(int num_of_pubs, int** publisher_fd, int num_of_subs, int** subscriber_data_fd, int** subscriber_notify_fd, int** matching_table)
{
//******************** dynamic memory allocation********************************************

      pubs_num = num_of_pubs; // set the attribute number of publishers

// Allocate a 2D array to store the two file descriptor of each publisher pipe 
      pubs_filedes = new int*[num_of_pubs]; 

      for (int i = 0; i < num_of_pubs; i++)
      {
            pubs_filedes[i] = new int[2];
      }

      subs_num = num_of_subs; // set the attribute number of subscribers

// Allocate two  2D array to store the two file descriptor of each subscriber pipes (notify pipe and data pipe) 
      subs_notify_filedes = new int*[num_of_subs]; // notify pipe 2D array of file descriptors
      subs_data_filedes = new int*[num_of_subs]; // data pipe 2D array of file descriptors
      for (int i = 0; i < num_of_subs; i++)
      {
              subs_data_filedes[i] = new int[2];
              subs_notify_filedes[i] = new int[2];
      }
// Allocate a 2D array to store the matching table between subscribers and publishers, since a subscriber can subscribe to more topics published by different publishers 
      subs_to_pubs_matching_table = new int*[num_of_subs];
      for (int i = 0; i < num_of_subs; i++)
      {
             subs_to_pubs_matching_table[i] = new int[num_of_pubs];
      }   
// Allocate an array to store as many buffers as publishers
      buffer = new Queue[num_of_pubs];
        
//*****************************initialization************************************************

// Initialize the 2D array of publishers' file descriptors and close the writing ones
      for (int i = 0; i < num_of_pubs; i++)
      {
              for (int j = 1; j < 2; j++)
              {
                      pubs_filedes[i][j] = publisher_fd[i][j];
              }
              close(pubs_filedes[i][1]); // close writing file descriptor of i-th publisher pipe
      }

// Initialize the 2D array of subscribers' file descriptors: close the notify pipes writing file descriptors and the data pipes reading ones 
      for (int i = 0; i < num_of_subs; i++)
      {
              for (int j = 1; j < 2; j++)
              {
                      subs_data_filedes[i][j] = subscriber_data_fd[i][j];
                      subs_notify_filedes[i][j] = subscriber_notify_fd[i][j];
              }
              close(subs_data_filedes[i][0]); // close reading file descriptor of i-th subscriber data pipe
              close(subs_notify_filedes[i][1]); // close writing file descriptor of i-th subscriber notif pipe
      }
// Initialize the 2D array matching table which says which subscriber (row) is subscribed to which publisher (column)  
      subs_to_pubs_matching_table = matching_table;

// Initialize the buffers based on the matching table informations
      int subscriptions_counter[num_of_pubs] = {0}; // number of subscribers which are subscribed to the topic published by the i-th publisher
      for (int i = 0; i < num_of_pubs; i++) // for each publisher
      {
              for (int j = 0; j < num_of_subs; j++) // for each subscriber
              {
                      if (matching_table[j][i] == 1) // if the j-th subscriber is subscribed to the topic published by the i-th publisher 
                      {
                              subscriptions_counter[i]++; // increase the subscriptions counter of topic published by the i-th publisher
                      }
              }
              buffer[i].set_attributes(BUFFER_SIZE,subscriptions_counter[i]); // contsruct the buffer relative to the topic published by the i-th publisher         
      }        

}

Mediator::~Mediator()
{
    delete subs_to_pubs_matching_table;
    delete pubs_filedes;
    delete subs_notify_filedes;
    delete subs_data_filedes;
    delete buffer;
}

void Mediator::fromPubs_checkNotify_BufToSubs() // control if any publisher has written a new char in its own pipe and if that is the case add it to the correspondig buffer + check if a subscriber ha notified that it wants new data and in that case sends it the newest data sent by the publisher it is subscribed to (which is stored in a circular buffer); 
{
        char new_data;
        fd_set pubs_read_fildes_set, notify_read_filedes_set, data_write_filedes_set; /// declare set of file descriptors
        FD_ZERO(&pubs_read_fildes_set); // initialize the set of reading from publishers'pipe file descriptors
        FD_ZERO(&notify_read_filedes_set); // initialize the set of reading from subscribers' notify pipes 
        FD_ZERO(&data_write_filedes_set); // initialize the set of writing into subscribers'data pipes
        while(1) 
        {
                for (int i = 0; i < pubs_num; i++) // assign the reading file descriptor of publisher pipes to the reading file descriptors set
                {
                        FD_SET(pubs_filedes[i][0], &pubs_read_fildes_set);
                }

                for (int i = 0; i < subs_num; i++) // for all subscribers
                {
                        FD_SET(subs_notify_filedes[i][0], &notify_read_filedes_set); // add the notify reading file descriptor of the i-th subscriber to the set
                        FD_SET(subs_data_filedes[i][1], &data_write_filedes_set); // add the data writing file descriptor of the i-th subscriber to the set
                }

                select(max_positive_in_column_2D_array(pubs_filedes, 0) + 1, &pubs_read_fildes_set, NULL, NULL, NULL); // check for new data in the pipes
                select(max_positive_in_column_2D_array(subs_notify_filedes, 0) + 1, &notify_read_filedes_set, NULL, NULL, NULL); // check if there's new data in the i-th subscriber notify pipe
                select(max_positive_in_column_2D_array(subs_data_filedes, 1) + 1, NULL, &data_write_filedes_set, NULL, NULL); // control if it is possible to write in the i-th subscriber data pipe

                for (int i = 0; i < pubs_num; i++) // for all pipes
                {
                        if ( FD_ISSET(pubs_filedes[i][0], &pubs_read_fildes_set) ) // if there is new data in the i-th publisher pipe 
                        {   
                                read(pubs_filedes[i][0],&new_data,sizeof(char)); // read a char from the i-th publisher pipe
                                buffer[i].enQueue(new_data); // add the char read from the i-th publisher pipe to the corresponding i-th buffer
                        }
                }

                for (int i = 0; i < subs_num; i++) // for all subscribers
                {
                        if ( FD_ISSET(subs_notify_filedes[i][0], &notify_read_filedes_set) ) // if the i-th subscriber sent a request for new data
                        {
                                if ( FD_ISSET(subs_data_filedes[i][1], &data_write_filedes_set) ) // if it is possible to write in the i-th subscriber data pipe 
                                {
                                        for (int j = 0; j < pubs_num; j++) // for all publishers
                                        {
                                                if (subs_to_pubs_matching_table[i][j] == 1) // if i-th subscriber is subscribed to j-th publisher
                                                {
                                                        int data = buffer[j].deQueue(i); // get data from the j-th circular buffer(j-th publisher) to be sent to the i-th subscriber 
                                                        write(subs_data_filedes[i][1], &data, sizeof(char)); // write the data in the i-th subscriber data pipe 
                                                }
                                        }
                                }
                                else  // if it is not possible to write in the i-th subscriber data pipe 
                                {
                                        cout << "No space available to write in " << i << "-th subscriber data pipe" << endl;
                                }
                        }
                }
        }
}


                       
