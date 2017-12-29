#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include "../SubscriptionTable.h"
#include "../auxiliary_functions.h"
#include "../Publisher2_0.h"

using namespace std;

#define PUB_PERIOD 3
#define CHARACTER_PARAM 'A'
int main (int argc, char* argv[])
{
        int pubs_num;
        int pub_id;
        char** fd_str_array;
        int fd_str_array_length;
        int* pubs_mode;
        int* pubs_period;

        pubs_num = atoi(argv[2]);
        fd_str_array_length = 1 + 2*pubs_num;
        fd_str_array = new char*[fd_str_array_length];
        pubs_mode = new int[pubs_num];
        pubs_period = new int[pubs_num];

        int i = 2;
        int q = 0;
        while ( q < fd_str_array_length)
        {
                fd_str_array[q] = argv[i];
                i++;
                q++;
        }

        q = 0;
        while ( q < pubs_num );
        {
                pubs_mode[q] = atoi(argv[i]);
                i++;
                q++;
        }

        q = 0;
        while(q < pubs_num)
        {
                pubs_period[q] = atoi(argv[i]);
                i++;
                q++;
        }

        pub_id = atoi(argv[1]);
        vector<vector<int> > pub_fd_2Dtable;
        pub_fd_2Dtable = fromStrArrayTo2DVecFds(fd_str_array);
        Publisher pub(pubs_period[pub_id], pub_fd_2Dtable[pub_id][0], pub_fd_2Dtable[pub_id][1]);
        
        char char_param;
        if ( pubs_mode[pub_id] == 0)
        {
                char_param = 'a';
        }
        if ( pubs_mode[pub_id] == 1)
        {
                char_param = 'A';
        }

        pub.write_in_pipe(char_param);
}
