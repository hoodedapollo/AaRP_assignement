#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../SimpleSubscriber.h"
#include <sys/types.h>
#include <string>
#include <vector>

using namespace std;

#define SUB_PERIOD 15 // ms
#define NOTIFY_MSG 2 

int main(int argc, char* argv[])
{
        cout << "SUB3 after execve" << endl;
        int notify_fd[2];
        int data_fd[2];

        notify_fd[0] = atoi(argv[1]);
        notify_fd[1] = atoi(argv[2]);
        data_fd[0] = atoi(argv[3]);        
        data_fd[1] = atoi(argv[4]);        

        SimpleSubscriber sub(3, SUB_PERIOD, notify_fd, data_fd, NOTIFY_MSG);
        sub.notify_and_read();
        
}
