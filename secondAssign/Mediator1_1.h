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
                int pipes_counter;
                vector<vector<int> > subs_to_pubs_matching_table; // bidimensional array containing the subs id and the id of the publishers it is subscribed to. 
                vector<vector<int> > pubs_filedes; // array of file descriptors of the same pipe: pubs_fildes[i][0] is the reading file descriptor of the i-th publisher pipe, while pubs_fildes[i][1] is the writing one 
                vector<vector<vector<int> > > subs_notify_filedes; // same as pubs_fildes but for subscribers' notify pipes
                vector<vector<vector<int> > > subs_data_filedes; // same as *pubs_fildes but for subscribers' readfrom pipes
                Queue* buffer;
        public:
                Mediator(int num_of_pubs, vector<vector<int> > pubslisher_fd, int num_of_subs, vector<vector<vector<int> > > subscriber_data_fd, vector<vector<vector<int> > >subscriber_notify_fd, vector<vector<int> > matching_table);
                void fromPubs_checkNotify_BufToSubs(); // check if a subscriber ha notified that it wants new data and in that case sends it the newest data sent by the publisher it is subscribed to (which is stored in a circular buffer); 
};


Mediator::Mediator(int num_of_pubs, vector<vector<int> > pubslisher_fd, int num_of_subs, vector<vector<int> > subscriber_data_fd, vector<vector<int> > subscriber_notify_fd, vector<vector<int> > matching_tableiint num_of_pubs, vector<vector<int> > pubslisher_fd, int num_of_subs, vector<vector<int> > subscriber_data_fd, vector<vector<int> > subscriber_notify_fd, vector<vector<int> > matching_table)
{
//******************** dynamic memory allocation********************************************

      pubs_num = num_of_pubs; // set the attribute number of publishers
      subs_num = num_of_subs; // set the attribute number of subscribers
        
      // vector<int> table_row(num_of_pubs,0)
      // subs_to_pubs_matching_table.resize(num_of_subs,table_row);
      
      subs_to_pubs_matching_table = matching_table;

      // pipes_counter = 0;
      // for (unsigned int i = 0; i < num_of_subs; i++)
      // {
      //         for (unsigned int j = 0; j < num_of_pubs; j++)
      //         {
      //                if (matching_table[i][j] = 1)
      //                {
      //                        pipes_counter++;
      //                }
      //         }
      // }
      //
      // vector<int> row_fd(2,0);
      // pubs_filedes.resize(num_of_pubs,row_fd);
      // subs_data_filedes.resize(pipes_counter, row_fd);
      // subs_notify_filedes.resize(pipes_counter, row_fd);

      pubs_filedes = pubslisher_fd;
      for (int i = 0; i < num_of_pubs; i++)
      {
              close(pubs_filedes[i][1]); // close writing file descriptor of i-th publisher pipe
      }

      subs_data_filedes = subscriber_data_fd;
      subs_notify_filedes = subscriber_notify_fd;
      for (unsigned int i = 0; i < subscriber_data_fd.size(); i++)
      {
              for(unsigned int j = 0; j < subscriber_data_fd[i].size(); j++)
              close(subs_data_filedes[i][j][0]);
              close(subs_notify_filedes[i][j][1]);
      }              

// Allocate an array to store as many buffers as publishers
      buffer = new Queue[num_of_pubs];
        
// Initialize the buffers based on the matching table informations
      vector<int> subscriptions_counter(num_of_pubs,0); // number of subscribers which are subscribed to the topic published by the i-th publisher
     for (unsigned int i = 0; i < subscriber_data_fd.size(); i++)
     {
             subscriptions_counter[i] = subscriber_data_fd[i].size();
     }

      // for (int i = 0; i < num_of_pubs; i++) // for each publisher
      // {
      //         for (int j = 0; j < num_of_subs; j++) // for each subscriber
      //         {
      //                 if (matching_table[j][i] == 1) // if the j-th subscriber is subscribed to the topic published by the i-th publisher
      //                 {
      //                         subscriptions_counter[i]++; // increase the subscriptions counter of topic published by the i-th publisher
      //                 }
      //         }
               
              buffer[i].set_attributes(BUFFER_SIZE,subscriptions_counter[i]); // contsruct the buffer relative to the topic published by the i-th publisher         
      }        

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

                for (unsigned int i = 0; i < subs_data_filedes.size(); i++) // for all subscribers
                {
                        for (unsigned int j = 0; j < subs_data_filedes[i].size(); j++)
                        {
                                FD_SET(subs_notify_filedes[i][j][0], &notify_read_filedes_set); // add the notify reading file descriptor of the i-th subscriber to the set
                                FD_SET(subs_data_filedes[i][j][1], &data_write_filedes_set); // add the data writing file descriptor of the i-th subscriber to the set
                        }
                }

                select(max_positive_in_column_2D_array(pubs_filedes, 0) + 1, &pubs_read_fildes_set, NULL, NULL, NULL); // check for new data in the pipes
                select(max_positive_in_column_2D_array(subs_notify_filedes, 0) + 1, &notify_read_filedes_set, NULL, NULL, NULL); // check if there's new data in the i-th subscriber notify pipe
//                select(max_positive_in_column_2D_array(subs_data_filedes, 1) + 1, NULL, &data_write_filedes_set, NULL, NULL); // control if it is possible to write in the i-th subscriber data pipe

                for (int i = 0; i < pubs_num; i++) // for all pipes
                {
                        if ( FD_ISSET(pubs_filedes[i][0], &pubs_read_fildes_set) ) // if there is new data in the i-th publisher pipe 
                        {   
                                read(pubs_filedes[i][0],&new_data,sizeof(new_data)); // read a char from the i-th publisher pipe
                                buffer[i].enQueue(new_data); // add the char read from the i-th publisher pipe to the corresponding i-th buffer
                        }
                }

                for (unsigned int i = 0; i < subs_data_filedes.size(); i++) // for all subscribers
                {
                        for (unsigned int j = 0; j < subs_data_filedes[i].size(); j++)
                        {
                                if ( FD_ISSET(subs_notify_filedes[i][j][0], &notify_read_filedes_set) ) // if a request for new data was sent trough the i-th pipe
                                {
                                        //                                if ( FD_ISSET(subs_data_filedes[i][1], &data_write_filedes_set) ) // if it is possible to write in the i-th subscriber data pipe 
                                        //                                {
                                        for (int j = 0; j < pubs_num; j++) // for all publishers
                                        {
                                                if (subs_to_pubs_matching_table[i][j] == 1) // if i-th subscriber is subscribed to j-th publisher
                                                {
                                                        int data = buffer[j].deQueue(i); // get data from the j-th circular buffer(j-th publisher) to be sent to the i-th subscriber 
                                                        write(subs_data_filedes[i][1], &data, sizeof(char)); // write the data in the i-th subscriber data pipe 
                                                }
                                        }
                                        //                                }
                                        //                                else  // if it is not possible to write in the i-th subscriber data pipe 
                                        //                                {
                                        //                                        cout << "No space available to write in " << i << "-th subscriber data pipe" << endl;
                                        //                                }
                                }
                        }
                }
        }
}



