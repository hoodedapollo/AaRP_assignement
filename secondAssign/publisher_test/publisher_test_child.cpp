#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include "../Publisher2_0.h"

using namespace std;

int main(int argc, char* argv[])
{
        int filedes[2];
        Publisher pub1(5,filedes[0],filedes[1]);

        filedes[0] = atoi(argv[1]);
        filedes[1] = atoi(argv[2]);

        pub1.write_in_pipe('a');
}
