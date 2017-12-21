#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

class Mediator 
{
        private:
                int pubs_num; // number of publishers which is equal to the number of buffers they respectively write into
                int subs_num; // number of subscribers
                int** subs_to_pubs_matching_table; // bidimensional array containing the subs id and the id of the publishers it is subscribed to. 
                int** pubs_filedes; // array of file descriptors of the same pipe: pubs_fildes[i][0] is the reading file descriptor of the i-th publisher pipe, while pubs_fildes[i][1] is the writing one 
                int** subs_notify_filedes; // same as *pubs_fildes but for subscribers' notify pipes
                int *subs_data_filedes; // same as *pubs_fildes but for subscribers' readfrom pipes
                // class circular_buffer* buffer;
        public:
                Mediator(int);
                void pubs_to_buffer(); // control if any publisher has written a new char in its own pipe and if that is the case add it to the correspondig buffer
                void check_notify_and_buf_to_subs(); // check if a subscriber ha notified that it wants new data and in that case sends it the newest data sent by the publisher it is subscribed to (which is stored in a circular buffer); 
};


Mediator::Mediator(int num_of_pubs, int** pubslisher_fd, int num_of_subs, int** subscriber_data_fd, int** subscriber_notify_fd, int** matching_table)
{
//******************** dynamic memory allocation********************************************

      pubs_num = num_of_pubs; // set the attribute number of publishers

// Allocate a 2D array to store the two file descriptor of each publisher pipe 
      pubs_filedes = new int*[pubs_num]; 

      for (int i = 0; i < pubs_num; i++)
      {
            pubs_filedes[i] = new int[2];
      }

      subs_num = num_of_subs; // set the attribute number of subscribers

// Allocate two  2D array to store the two file descriptor of each subscriber pipes (notify pipe and data pipe) 
      subs_notify_filedes = new int*[subs_num]; // notify pipe 2D array of file descriptors
      subs_data_filedes = new int*[subs_num]; // data pipe 2D array of file descriptors
      for (int i = 0; i < subs_num; i++)
      {
              subs_data_filedes[i] = new int[2];
              subs_notify_filedes[i] = new int[2];
      }
// Allocate a 2D array to store the matching table between subscribers and publishers, since a subscriber can subscribe to more topics published by different publishers 
      subs_from_buffer = new int[subs_num];
      for (int i = 0; i < subs_num; i++)
      {
             subs_to_pubs_matching_table[i] = new int[pubs_num];
      }   
// Allocate an array to store as many buffers as publishers
      // buffer = new class circular_buffer[pubs_num];
        
//*****************************initialization************************************************

// Initialize the 2D array of publishers' file descriptors and close the writing ones
      for (int i = 0; i < pubs_num; i++)
      {
              for (int j = 1; j < 2; j++)
              {
                      pubs_filedes[i][j] = publisher_fd[i][j];
              }
              close(pubs_filedes[i][1]); // close writing file descriptor of i-th publisher pipe
      }

// Initialize the 2D array of subscribers' file descriptors: close the notify pipes writing file descriptors and the data pipes reading ones 
      for (int i = 0; i < subs_num; i++)
      {
              for (int j = 1; j < 2; j++)
              {
                      subs_data_filedes[i][j] = subscrber_data_fd[i][j];
                      subs_notify_filedes[i][j] = subscriber_notify_fd[i][j];
              }
              close(subs_data_filedes[i][0]); // close reading file descriptor of i-th subscriber data pipe
              close(subs_notify_filedes[i][1]); // close writing file descriptor of i-th subscriber notif pipe
      }
// Initialize the 2D array matching table which says which subscriber (row) is subscribed to which publisher (column)  
      subs_to_pubs_matching_table = matching_table;
}

void pubs_to_buffer()  // control if any publisher has written a new char in its own pipe and if that is the case add it to the correspondig buffer   
{
        char new_data;
        fd_set pubs_read_fildes_set; // declaration  of reading file descriptors set
        FD_ZERO(&pubs_read_fildes_set); // initialization of reading file descriptors set
        for (int i = 0; i < pubs_num; i++) // assign the reading file descriptor of publisher pipes to the reading file descriptors set
        {
                FD_SET(pubs_fildes[i][0], &pubs_read_fildes_set);
        }
        select(max_positive_in_column_2D_array(pubs_filedes, 0) + 1, &pubs_read_fildes_set, NULL, NULL, NULL); // check for new data in the pipes
        for (int i = 0; i < pubs_num; i++) // for all pipes
        {
                if ( FD_ISSET(pubs_fildes[i][0], &pubs_read_fildes_set) ) // if there is new data in the i-th publisher pipe 
                {   
                        read(pubs_fildes[i][0],&new_data,sizeof(char)); // read a char from the i-th publisher pipe
                        //buffer[i].add = new_data; // add the char read from the i-th publisher pipe to the corresponding i-th buffer
                }
        }

}    

void check_notify_and_buf_to_subs() // check if a subscriber ha notified that it wants new data and in that case sends it the newest data sent by the publisher it is subscribed to (which is stored in a circular buffer); 
{
        fd_set notify_read_filedes_set, data_write_filedes_set; // decleare the two set of file descriptors
        FD_ZERO(&notify_read_filedes_set); // initialize the sets
        FD_ZERO(&data_write_filedes_set);
        while(1) 
        {
                for (int i = 0; i < subs_num; i++) // for all subscribers
                {
                        FD_SET(subs_notify_filedes[i][0], &notify_read_filedes_set); // add the notify reading file descriptor of the i-th subscriber to the set
                        FD_SET(subs_data_filedes[i][1], &data_write_filedes_set); // add the data writing file descriptor of the i-th subscriber to the set
                }

                select(max_positive_in_column_2D_array(subs_notify_filedes, 0) + 1, &notify_read_filedes_set, NULL, NULL, NULL); // check if there's new data in the i-th subscriber notify pipe
                select(max_positive_in_column_2D_array(subs_data_filedes, 1) + 1, NULL, &data_write_filedes_set, NULL, NULL); // control if it is possible to write in the i-th subscriber data pipe

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
                                                        // data = buffer[j].data; // get data from the j-th circular buffer which is the one in which the mediator stores data sent by the j-th publisher 
                                                        // write(subs_data_fildes[i][1], &data, sizeof(char)); // write the data in the i-th subscriber data pipe 
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


                       
