#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include "../SubscriptionTable.h"
//#include "../auxiliary_functions.h"
#include "../Mediator1_1.h"

using namespace std;

typedef vector<vector<int> > table2D;
typedef vector<vector<vector<int> > > table3D;

int main(int argc, char* argv[])
{
        int subs_num = atoi(argv[1]);
        int pubs_num = atoi(argv[2]);

        char** subs_data_fd_str_array;
        char** subs_notify_fd_str_array;
        char** pubs_fd_str_array;
        char** matching_str_array;

        int length_of_subs_fd_str_array = 2 + 2*(subs_num * pubs_num);
        int length_of_pubs_fd_str_array = 1 + 2*pubs_num;
        int length_of_matching_str_array = 2 + subs_num * pubs_num;

        subs_data_fd_str_array = new char*[length_of_subs_fd_str_array];
        subs_notify_fd_str_array = new char*[length_of_subs_fd_str_array];
        pubs_fd_str_array = new char*[length_of_pubs_fd_str_array];
        matching_str_array = new char*[length_of_matching_str_array];

        int i = 1;
        int q = 0;
        while(q < length_of_subs_fd_str_array)
        {
                subs_data_fd_str_array[q] = argv[i];
                i++;
                q++;
        }
        q = 0;
        while(q < length_of_subs_fd_str_array)
        {
                subs_notify_fd_str_array[q] = argv[i];
                i++;
                q++;
        }
        q = 0;
        while(q < length_of_pubs_fd_str_array)
        {
                pubs_fd_str_array[q] = argv[i];
                i++;
                q++;
        }
        q = 0;
        while(q < length_of_matching_str_array)
        {
                matching_str_array[q] = argv[i];
                i++;
                q++;
        }

        table3D subs_data_fd_3Dtable;
        table3D subs_notify_fd_3Dtable;
        table2D pubs_fd_2Dtable;
        table2D matching_table;

        subs_data_fd_3Dtable = fromStrArrayTo3DVecFds(subs_data_fd_str_array);
        subs_notify_fd_3Dtable = fromStrArrayTo3DVecFds(subs_notify_fd_str_array);
        pubs_fd_2Dtable = fromStrArrayTo2DVecFds(pubs_fd_str_array);
        matching_table = fromStrArrayTo2DVecMatchingTable(matching_str_array);

        Mediator mediator(pubs_fd_2Dtable, subs_data_fd_3Dtable, subs_notify_fd_3Dtable, matching_table);
        mediator.fromPubs_checkNotify_BufToSubs();

        return 0;
}

