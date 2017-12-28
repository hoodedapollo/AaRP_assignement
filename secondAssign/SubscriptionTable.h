#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <iostream>
#include <stdlib.h>

using namespace std;

class SubscriptionTable
{
        private:
                int pubs_num;
                int subs_num;
                vector<vector<int> > table;
                vector<vector<int> > vector_pubs_filedes; 
                vector<vector<vector<int> > > vector_data_filedes;
                vector<vector<vector<int> > > vector_notify_filedes;
        public:
                SubscriptionTable();
                SubscriptionTable(int num_of_subs, int num_of_pubs); // generate a table in which is specified which subscriber is subscribed to which pulisher's topic by asking the user for information, paramenters are the number of publishers and subscribers
                SubscriptionTable(vector<vector<vector<int> > > matching_table); // given the matching table as a paramenter is sets the number of subscribers as the number of rows of the table and the number of publishers as the number of columns of the first row of the table
                vector<vector<int> >getTable(); // returns the matching table
                int isSubscribedTo(int sub, int pub); // if subscriber sub is subscribed to th topic published by publisher pub it returns 1 otherway it returns 0
                vector<vector<int> > generatePublisherPipes();
                vector<vector<vector<int> > > generateDataPipes(); // generate the table of data pipes file descriptors based on the matching table built by the constructor
                vector<vector<vector<int> > > generateNotifyPipes(); // generate the table of notify pipes file descriptors based on the matching table built by the constructor

};


SubscriptionTable::SubscriptionTable() // generate a table in which is specified which subscriber is subscribed to which pulisher's topic by asking the user for information, paramenters are the number of publishers and subscribers
{ 
        string answer;

        cout << "How many publisher do you want?" << endl;
        cin >> pubs_num;
        cout << "How many subscriber do you want?" << "\n\n";
        cin >> subs_num;

        // table dynamic memory allocation
        table.resize(subs_num);
        for(int i = 0; i < subs_num; i++)
        {
                table[i].resize(pubs_num,0);
        } 

        // build the table asking the user for information
        for ( int i = 0; i < subs_num; i++ )
        { 
                for(int j = 0; j < pubs_num; j++)
                {
                        cout << "Is subscriber " << i << "subscribed to topic published by publisher " << j << "?\n"; 
                        cin >> answer;
                        if (answer.compare("y") == 0 || answer.compare("yes") ==0)
                        {
                                table[i][j] = 1;
                        }
                        else
                        {
                                table[i][j] = 0;
                        }
                }
        }   
}

SubscriptionTable::SubscriptionTable(int num_of_subs, int num_of_pubs) // generate a table in which is specified which subscriber is subscribed to which pulisher's topic by asking the user for information, paramenters are the number of publishers and subscribers
{ 
        string answer;

        subs_num = num_of_subs;
        pubs_num = num_of_pubs;

        // table dynamic memory allocation
        table.resize(num_of_subs);
        for(int i = 0; i < num_of_subs; i++)
        {
                table[i].resize(num_of_pubs,0);
        } 

        // build the table asking the user for information
        for ( int i = 0; i < num_of_subs; i++ )
        { 
                for(int j = 0; j < num_of_pubs; j++)
                {
                        cout << "Is subscriber " << i << "subscribed to topic published by publisher " << j << "?\n"; 
                        cin >> answer;
                        if (answer.compare("y") == 0 || answer.compare("yes") ==0)
                        {
                                table[i][j] = 1;
                        }
                        else
                        {
                                table[i][j] = 0;
                        }
                }
        }   
}

SubscriptionTable::SubscriptionTable(vector<vector<vector<int> > > matching_table) // given the matching table as a paramenter is sets the number of subscribers as the number of rows of the table and the number of publishers as the number of columns of the first row of the table
{
        subs_num = matching_table.size();
        pubs_num = matching_table[0].size(); 

}

vector<vector<int> > SubscriptionTable::getTable()
{ return table;}

int SubscriptionTable::isSubscribedTo(int sub, int pub) // if subscriber sub is subscribed to th topic published by publisher pub it returns 1 otherway it returns 0
{
        if (table[sub][pub] == 1)
        { 
                return 1;
        }
        else
        {
                return 0;
        }
}

vector<vector<int> > SubscriptionTable::generatePublisherPipes()
{
        vector<int> two_fd(2,0);
        vector_pubs_filedes.resize(pubs_num, two_fd);

        for (int i = 0; i < pubs_num; i++)
        {
                pipe(vector_pubs_filedes[i].data());
        }
        return vector_pubs_filedes;
}

vector<vector<vector<int> > > SubscriptionTable::generateDataPipes() // generate the table of data pipes file descriptors based on the matching table built by the constructor
{
        // declare a 3D vector based on the number of subscribers and publishers ( 2 file descriptors per pipe)
        vector<int> two_fd(2,0);
        vector<vector<int> > pubs_row(pubs_num, two_fd);
        vector_data_filedes.resize(subs_num,pubs_row);

        // for the element in the matching table
        for (int i = 0; i < subs_num; i++)
        {
                for (int j = 0; j < pubs_num; j++)
                { 
                        if (table[i][j] == 1) // if the i-th subscriber is subscribed to the topic published by the j-th publisher
                        {
                                pipe(vector_data_filedes[i][j].data()); // generate a pipe and store its file descriptors in vector_data_filedes[i][j][0] (reading) and in vector_data_filedes[i][j][1] (writing)                                                     
                        }
                }
        }
        return vector_data_filedes;
}
vector<vector<vector<int> > > SubscriptionTable::generateNotifyPipes() // generate the table of notify pipes file descriptors based on the matching table built by the constructor
{
        vector<int> two_fd(2,0);
        vector<vector<int> > pubs_row(pubs_num, two_fd);
        vector_notify_filedes.resize(subs_num,pubs_row);
        for (int i = 0; i < subs_num; i++)
        {
                for (int j = 0; j < pubs_num; j++)
                { 
                        if (table[i][j] == 1)
                        {
                                pipe(vector_notify_filedes[i][j].data());                                                    
                        }
                }
        }
        return vector_notify_filedes;
}
