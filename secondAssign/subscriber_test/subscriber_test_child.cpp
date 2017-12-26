#include <string.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <sys/types.h>
#include <sys/wait.h>
#include <vector>
#include "../Subscriber4_2.h"

using namespace std;

#define N_PIPES 2
#define SUB_PERIOD 2
#define NOTIFY_MSG 1
int main (int argc, char* argv[])
{
        vector<char> char_read(N_PIPES,0);
        vector<int> row_fd(2,0);
        vector<vector<int> > vector_data_filedes(N_PIPES,row_fd);
        vector<vector<int> > vector_notify_filedes(N_PIPES,row_fd);

        unsigned int q = 0;
        int j = 1;
            
        while (q < vector_data_filedes.size())
        {
                vector_data_filedes[q][0] = atoi(argv[j]);
                j++;
                vector_data_filedes[q][1] = atoi(argv[j]);
                j++;
                q++;
        }
        
        q = 0;
        while(q < vector_notify_filedes.size())
        {
                vector_notify_filedes[q][0] = atoi(argv[j]);
                j++;
                vector_notify_filedes[q][1] = atoi(argv[j]);
                j++;
                q++;
        }
        cout << "CHLD:" << endl;
        for (int i = 0; i < N_PIPES; i++)
        {
                for (int j = 0; j < 2; j++)
                {
                        cout << "data pipe int fd: vector_data_filedes[" << i << "][" << j << "] --> " << vector_data_filedes[i][j] << endl;
                        cout << "notify pipe int fd: vector_notify_filedes[" << i << "][" << j << "] --> " << vector_notify_filedes[i][j] << endl << endl;
                }
        }
        Subscriber sub1(SUB_PERIOD, N_PIPES, vector_data_filedes, vector_notify_filedes);
        sub1.notify_and_read(NOTIFY_MSG); 
}
                
