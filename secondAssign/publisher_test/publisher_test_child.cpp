#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include "../Publisher2_0.h"

using namespace std;

#define PUB_PERIOD 5 

int main(int argc, char* argv[])
{
        int filedes[2];

        filedes[0] = atoi(argv[1]);
        cout << "CHILD: fildes[0] -->" << filedes[0] << endl;
        filedes[1] = atoi(argv[2]);
        cout << "CHILD: fildes[1] -->" << filedes[1] << endl;
        
        Publisher pub1(PUB_PERIOD,filedes[0],filedes[1]); // declare a publisher with a publishing period of PUB_PERIOD seconds, which  writes in the file which file descriptor is fildes[1] and closes the reading file descriptor fildes[0] 
        pub1.write_in_pipe('A'); // write a single char (by default) each publishing period: 'A' means capitol letter, 'a' means normal letter.
}
