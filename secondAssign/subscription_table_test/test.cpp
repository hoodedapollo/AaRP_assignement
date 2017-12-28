#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>
#include "../SubscriptionTable.h"

#define SUBS_NUM 3
#define PUBS_NUM 2

int main()
{
        SubscriptionTable tab1(SUBS_NUM,PUBS_NUM);
        vector<vector<int> >  matching_table;
        matching_table = tab1.getTable();
        cout << "matching table\n";
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

        cout << "\ndata pipe reading file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << data_filedes_table[i][j][0] << " ";
                }
                cout << endl;
        }

        cout << "\ndata pipe writing file descriptors" << "\n\n";
        for (int i = 0; i < SUBS_NUM; i++)
        {
                for (int j = 0; j < PUBS_NUM; j++)
                {
                        cout << data_filedes_table[i][j][1] << " ";
                }
                cout << endl;
        }
        return 1;
} 
