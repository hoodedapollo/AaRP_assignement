#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../SimpleMediator.h"

using namespace std;

#define PUB_NUM 2
#define SUB_NUM 3

int main(int argc, char* argv[])
{
        cout << "MED after execve" << endl;
        int pub_fd[PUB_NUM][2];
        int sub_data_fd[SUB_NUM][2];
        int sub_notify_fd[SUB_NUM][2];


        int q = 1;
        int k = 0;
        while(k < PUB_NUM )
        {
                pub_fd[k][0] = atoi(argv[q]);
                q++;
                pub_fd[k][1] = atoi(argv[q]);
                q++;
                k++;
        }

        k = 0;
        while( k < SUB_NUM)
        {
                sub_notify_fd[k][0] = atoi(argv[q]);
                q++;
                sub_notify_fd[k][1] = atoi(argv[q]);
                q++;
                sub_data_fd[k][0] = atoi(argv[q]);
                q++;
                sub_data_fd[k][1] = atoi(argv[q]);
                q++;
                k++;
        }
        
        for (int i = 0; i < PUB_NUM; i++)
        {
                cout << "MED pud_fd["<<i<<"][0] = " << pub_fd[i][0] << endl;
                cout << "MED pub_fd["<<i<<"][1] = " << pub_fd[i][1] << endl;
        }

        SimpleMediator med(pub_fd, sub_notify_fd, sub_data_fd);
        med.fromPubs_checkNotify_BufToSubs();

        return 0;
}

