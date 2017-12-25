#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>

using namespace std;

#define N_ROWS

int main (int argc, char* argv[])
{
        vector<int> row_fd(2,0);
        vector<vector<int> > vector_fildes(N_ROWS,row_fd);
        unsigned int q = 0;
        int j = 1;
            
        while (q < vector_fildes.size())
        {
                vector_fildes[q][0] = argv[j];
                j++;
                vector_fildes[q][1] = argv[j];
                j++;
                q++;
        }


                
