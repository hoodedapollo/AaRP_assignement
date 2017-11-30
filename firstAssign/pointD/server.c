#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointD_lib.h"


int main (int argc, char *argv[]) {
        int w[10];      
        int fildes_firstpipe[2];
        int fildes_secondpipe[2];
        sighandler_t signal_sigusr1_ret, signal_sigusr2_ret;
        ssize_t bytes_read[10];     // array which stores number of bytes read during each reading
        int total_bytes_read;
        int pipe1_close_ret, pipe2_close_ret;

        signal_sigusr1_ret = signal(SIGUSR1,SIG_IGN);
        log_func("CHILD2 signal SIGUSER1 recieved");
        signal_err_control(signal_sigusr1_ret,"CHILD2: signal SIGUSR1");
        signal_sigusr2_ret = signal(SIGUSR2, handler); 
        log_func("CHILD2 signal SIGUSER2 recieved");
        signal_err_control(signal_sigusr2_ret,"CHILD2: signal SIGUSR2");

        fildes_firstpipe[0] = atoi(argv[1]);  // convert string pipes file descriptors pointed by argv into integers
        fildes_firstpipe[1] = atoi(argv[2]);
        fildes_secondpipe[0] = atoi(argv[3]);
        fildes_secondpipe[1] = atoi(argv[4]);

        pipe1_close_ret = close(fildes_firstpipe[1]);  // close writing side of the first  pipe before entering the loop
        log_func("CHILD2 close first pipe writing side");
        err_control(pipe1_close_ret,"CHILD2: writing pipe1 close",0);
        pipe2_close_ret = close(fildes_secondpipe[0]);  // close the reading side of the second pipe before entering the loop
        log_func("CHILD2 close second pipe reading side");
        err_control(pipe2_close_ret,"CHILD2: reading pipe2 close",0);

        while (1)
        {

//======= read unsorted vector from first pipe =======================================================================================

                total_bytes_read = 0;  // initialize total_bytes read each time before reading from pipe
                size_t w_size = sizeof((w[0]));
                for (int i = 0; i<10; i++) 
                {
                        bytes_read[i] = read(fildes_firstpipe[0], &w[i], w_size);
                        log_func("CHILD2 first pipe i-th read");
                        err_control(bytes_read[i], "CHILD2: first pipe read",0);
                        total_bytes_read = total_bytes_read + bytes_read[i];
                        if (bytes_read[i] < w_size)
                        {
                                printf("\nCHILD2: PARTIAL READ: bytes read by the i-th read -->  %ld\n",bytes_read[i]); fflush(stdout);
                        }
                }

                printf("\n   Total number of bytes read --> %d\n", total_bytes_read); fflush(stdout);

                if (total_bytes_read < 10 * w_size)
                {
                        printf("CHILD2: PARTIAL READING\n"); fflush(stdout);
                }
                else
                        printf("CHILD2: READING COMPLETED\n"); fflush(stdout);

//======= finish reading unsorted vector ============================================================================================

                bubble_sort(w);  // sort the vector read from pipe

//======= write sorted vector into second pipe=======================================================================================

                size_t w_totsize = sizeof(w);
                printf("\nCHILD2: WRITING IN THE PIPE...\n   size of vector v -->  %ld",w_totsize); fflush(stdout);
                ssize_t bytes_written = write(fildes_secondpipe[1], w, w_totsize);
                log_func("CHILD2 second pipe write");
                err_control(bytes_written,"CHILD2: second pipe write",0);
                printf("\n   bytes written -->  %ld\n", bytes_written); fflush(stdout);
                if (bytes_written == w_totsize)  // all data has been written
                {
                        printf("CHILD2: WRITING COMPLETED\n"); fflush(stdout);
                }
                else
                {
                        printf("CHILD2: WRITING NOT COMPLETED\n"); fflush(stdout);
                }
        }

}


