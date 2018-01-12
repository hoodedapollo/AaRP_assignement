#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../SimpleSubscriber.h"
#include <sys/types.h>
#include <string>
#include <vector>

using namespace std;

#define SUB_PERIOD 10 // ms
#define NOTIFY_MSG 1

int main(int argc, char* argv[])
{
        cout << "SUB2 after execve" << endl;
        int notify_fd[2];
        int data_fd[2];

        notify_fd[0] = atoi(argv[1]);
        notify_fd[1] = atoi(argv[2]);
        data_fd[0] = atoi(argv[3]);        
        data_fd[1] = atoi(argv[4]);        
        
        for (int i = 0; i < 2; i++)
        {
                cout << "       SUB2 Notify fd" << i << " -->  " << notify_fd[i] << "   correspondig argv --> " << argv[i+1] << endl;
                cout << "       SUB2 Data fd" << i << " -->  " << data_fd[i] << "   corresponding argv --> " << argv[i+3] << endl;
        }

        SimpleSubscriber sub(2, SUB_PERIOD, notify_fd, data_fd, NOTIFY_MSG);
        sub.notify_and_read();
        
}
