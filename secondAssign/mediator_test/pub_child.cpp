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
        int pubs_num = atoi(argv[2]);
        int pub_id;
        char** fd_str_array;
        int fd_str_array_length = 1 + 2*pubs_num;

        fd_str_array = new char*[fd_str_array_length];

        for (int i = 0; i < fd_str_array_length; i++)
        {
        fd_str_array[i] = argv[i+2];
        }

        pub_id = atoi(argv[1]);
        vector<vector<int> > pub_fd_2Dtable;
        pub_fd_2Dtable = fromStrArrayTo2DVecFds(fd_str_array);
        Publisher pub(PUB_PERIOD,pub_fd_2Dtable[pub_id][0], pub_fd_2Dtable[pub_id][1]);
        pub.write_in_pipe(CHARACTER_PARAM);
}
