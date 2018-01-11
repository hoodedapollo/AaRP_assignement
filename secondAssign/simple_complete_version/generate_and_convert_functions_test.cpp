#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "../SubscriptionTable.h"
#include "../auxiliary_functions.h"

#define SUBS_NUM 3
#define PUBS_NUM 2

int main()
{
        SubscriptionTable tab1(SUBS_NUM,PUBS_NUM);

// ********* 2D vector publisher pipes fildes
        vector<vector<int> > pubs_fildes_table;
        pubs_fildes_table = tab1.generatePublisherPipes();
        cout << "\nPublisher pipes reading fd table\n";
        for (int i = 0; i < PUBS_NUM; i++)
        {
               cout <<  pubs_fildes_table[i][0] << endl;
        }


        cout << "\nPublisher pipes writing fd table\n";
        for (int i = 0; i < PUBS_NUM; i++)
        {
               cout <<  pubs_fildes_table[i][1] << endl;
        }
        
//******* Matching Table ***************************************
        vector<vector<int> >  matching_table;
        matching_table = tab1.getTable();
        cout << "\nmatching table\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << matching_table[i][j] << " ";
                }
                cout << endl;
        }

//************* 3D vector subscriber data pipes fildes ******************************
        vector<vector<vector<int> > > data_filedes_table; 
        data_filedes_table = tab1.generateDataPipes();

        cout << "\ndata pipes reading file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << data_filedes_table[i][j][0] << " ";
                }
                cout << endl;
        }

        cout << "\ndata pipes writing file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << data_filedes_table[i][j][1] << " ";
                }
                cout << endl;
        }

//************** 3D Vector subscriber notify pipes filedes **************************
        vector<vector<vector<int> > > notify_filedes_table; 
        notify_filedes_table = tab1.generateNotifyPipes();

        cout << "\nnotify pipes reading file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << notify_filedes_table[i][j][0] << " ";
                }
                cout << endl;
        }

        cout << "\ninotify pipes writing file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << notify_filedes_table[i][j][1] << " ";
                }
                cout << endl;
        }

//****************** Array of string publishers' file descriptors********************
        cout <<"\narray of (string) publishers' pipes file descriptors\n ";
        
        char** array_of_pubs_fildes;
        array_of_pubs_fildes = pubsFdsFrom2DVecToStrArray(pubs_fildes_table);
        int pubs_array_length = 1 + 2*atoi(array_of_pubs_fildes[0]);
        for (int i = 0; i < pubs_array_length; i++)
        {
                cout << array_of_pubs_fildes[i] << " "; 
        }
        
        cout << endl;
//****************** Array of string data pipes file descriptors ********************       
        cout <<"\narray of (string) data pipes file descriptors\n ";

        char** array_of_data_fildes;
        array_of_data_fildes = subsFdsFrom3DVecToStrArray(data_filedes_table);
        int data_array_length = 2 + 2*(atoi(array_of_data_fildes[0]) * atoi(array_of_data_fildes[1]));
        for (int i = 0; i < data_array_length; i++)
        {
                cout << array_of_data_fildes[i] << " "; 
        }
        
        cout << endl;

//******************* array of string notify pipes file descriptors **********************        
        cout <<"\narray of (string) notify pipes file descriptors\n ";

        char** array_of_notify_fildes;
        array_of_notify_fildes = subsFdsFrom3DVecToStrArray(notify_filedes_table);
        int notify_array_length = 2 + 2*(atoi(array_of_notify_fildes[0]) * atoi(array_of_notify_fildes[1]));
        for (int i = 0; i < notify_array_length; i++)
        {
                cout << array_of_notify_fildes[i] << " "; 
        }
        
        cout << endl;

//****************** from array back to tables*******************************************
//
//****************** publishers' file descriptors****************************************

        vector<vector<int> > pubs_back_to_2Dtable;
        pubs_back_to_2Dtable = fromStrArrayTo2DVecFds(array_of_pubs_fildes);
        int pubs_num = atoi(array_of_pubs_fildes[0]);
        cout << "\nFrom pubs str array --> number of publishers " << pubs_num << endl;
        cout << "\nPublisher pipes reading fd table recovered from string array\n";
        for (int i = 0; i < pubs_num; i++)
        {
               cout <<  pubs_back_to_2Dtable[i][0] << endl;
        }


        cout << "\nPublisher pipes writing fd table recovered from string array\n";
        for (int i = 0; i < pubs_num; i++)
        {
               cout <<  pubs_back_to_2Dtable[i][1] << endl;
        }

//******************* subscribers' data pipe file descriptors
        
        vector<vector<vector<int> > > data_back_to_3Dtable;
        data_back_to_3Dtable = fromStrArrayTo3DVecFds(array_of_data_fildes);
        int subs_num = atoi(array_of_data_fildes[0]);
        pubs_num = atoi(array_of_data_fildes[1]);
        cout << "\nFrom data str array --> number of subscribers " << subs_num << endl;
        cout << "\nFrom data str array --> number of publishers " << pubs_num << endl;
        cout << "\ndata pipes reading file descriptors recovered from string array" << "\n\n";
        for (int i = 0; i < subs_num; i++)
        {
                for (int j = 0; j < pubs_num; j++)
                {
                        cout << data_back_to_3Dtable[i][j][0] << " ";
                }
                cout << endl;
        }

        cout << "\ndata pipes writing file descriptors recoverd from string array" << "\n\n";
        for (int i = 0; i < subs_num; i++)
        {
                for (int j = 0; j < pubs_num; j++)
                {
                        cout << data_back_to_3Dtable[i][j][1] << " ";
                }
                cout << endl;
        }
        
        return 0;
} 
