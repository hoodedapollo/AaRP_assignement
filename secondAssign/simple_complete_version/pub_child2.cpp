#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include "../Publisher2_0.h"

using namespace std;

#define PUB_PERIOD 3
#define CHARACTER_PARAM 'A'
int main (int argc, char* argv[])
{
        int fd[2];
        fd[0] = atoi(argv[1]);
        fd[1] = atoi(argv[2]);

        Publisher pub(PUB_PERIOD, fd[0], fd[1]);
        pub.write_in_pipe(CHARACTER_PARAM);
}
