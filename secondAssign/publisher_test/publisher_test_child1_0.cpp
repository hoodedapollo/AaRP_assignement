#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include "../Publisher2_0.h"

using namespace std;

#define PUB_PERIOD 1 

int main(int argc, char* argv[])
{
        int filedes[2];

        filedes[0] = atoi(argv[1]);
        filedes[1] = atoi(argv[2]);

        Publisher pub1(PUB_PERIOD,filedes[0],filedes[1]);
        pub1.write_in_pipe('a');
}
