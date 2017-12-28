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
        vector<vector<int> >  matching_table;
        matching_table = tab1.getTable();
        cout << "matching table\n";
        cout << "subs_num " << matching_table.size() << endl;
        cout << "pubs_num " << matching_table[0].size() << endl;
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << matching_table[i][j] << " ";
                }
                cout << endl;
        }

        vector<vector<vector<int> > > data_filedes_table; 
        data_filedes_table = tab1.generateDataPipes();

        cout << "filedes_table subs num " << data_filedes_table.size() << endl;
        cout << "filedes_table pubs num " << data_filedes_table[0].size() << endl;

        cout << "\nreading file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << data_filedes_table[i][j][0] << " ";
                }
                cout << endl;
        }

        cout << "\nwriting file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << data_filedes_table[i][j][1] << " ";
                }
                cout << endl;
        }

        cout <<"\narray of (string) file descriptors\n ";

        char** array_of_fildes;
        array_of_fildes = fromVecToArrayOfStringFds(data_filedes_table);
        int array_length = 2 + 2*(atoi(array_of_fildes[0]) * atoi(array_of_fildes[1]));
        for (int i = 0; i < array_length; i++)
        {
                cout << array_of_fildes[i] << " "; 
        }
        
        cout << endl;

        vector<vector<vector<int> > > table_from_array;
        table_from_array = fromCharToVecFds(array_of_fildes);

        cout << "\ntable_from_array num of subs " << table_from_array.size() << endl;
        cout << "table_from_array num of pubs " << table_from_array[0].size() << endl;
        

        cout << "\nreading file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << table_from_array[i][j][0] << " ";
                }
                cout << endl;
        }

        cout << "\nwriting file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << table_from_array[i][j][1] << " ";
                }
                cout << endl;
        }
        return 1;
} 
