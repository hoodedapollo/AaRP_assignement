#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include "../SimpleSubscriber.h"

using namespace std;

int main(int argc, char* argv[])
{
        int data_fd[2];
        int notify_fd[2];
        
        notify_fd[0] = atoi(argv[1]);
        notify_fd[1] = atoi(argv[2]);
        data_fd[0] = atoi(argv[3]);
        data_fd[1] = atoi(argv[4]);

        SimpleSubscriber sub(1, 3, notify_fd, data_fd, 1);
        sub.notify_and_read();

        return 0;
}
