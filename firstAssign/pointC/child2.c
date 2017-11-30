#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointC_lib.h"

int main (int argc, char *argv[]) {
        int w[10];      
        int fildes_firstpipe[2];
        int fildes_secondpipe[2];
        ssize_t bytes_read[10];     // array which stores number of bytes read during each reading
        int total_bytes_read = 0;

        fildes_firstpipe[0] = atoi(argv[1]);  // convert the file descriptors into numbers
        fildes_firstpipe[1] = atoi(argv[2]);

        fildes_secondpipe[0] = atoi(argv[3]);
        fildes_secondpipe[1] = atoi(argv[4]);

        int close_p1write_ret = close(fildes_firstpipe[1]); // before reading from first pipe close its writing side
        log_func("CHILD2 close first pipe write-side");
        err_control(close_p1write_ret,"CHILD2 close first pipe write-side: ",0);

        size_t w_size = sizeof(w[0]);
        for (int i = 0; i<10; i++) 
        {
                bytes_read[i] = read(fildes_firstpipe[0], &w[i], w_size);  // read, one by one, each element of the unsorted array
                log_func("CHILD2 i-th read");
                err_control(bytes_read[i],"CHILD2 i-th read: ",0);

                total_bytes_read = total_bytes_read + bytes_read[i];
                if (bytes_read[i] < w_size)
                {
                        printf("\nPARTIAL READ: bytes read by the i-th read -->  %ld\n",bytes_read[i]); fflush(stdout);
                }
        }

        printf("\n   Total number of bytes read --> %d\n", total_bytes_read); fflush(stdout);

        if (total_bytes_read < 10 * w_size)
        {
                printf("PARTIAL READING\n"); fflush(stdout);
        }
        else
                printf("READING COMPLETED\n"); fflush(stdout);



        int close_p1read_ret = close(fildes_firstpipe[0]);  // after reading from first pipe close its reading side 
        log_func("CHILD2 close first pipe read-side");
        err_control(close_p1read_ret,"CHILD2 close first pipe read-side: ",0);

        bubble_sort(w);  // sort the numbers in the array

        int close_p2read_ret = close(fildes_secondpipe[0]); // before writing close the reading side of the second pipe
        log_func("CHILD2 close second pipe read-side");
        err_control(close_p2read_ret,"CHILD2 close second pipe read-side: ",0);

        size_t w_totsize = sizeof(w);
        printf("\nWRITING IN THE PIPE...\n   size of vector v -->  %ld",w_totsize); fflush(stdout);
        ssize_t bytes_written = write(fildes_secondpipe[1], w, w_totsize);  // write the sorted array in the second pipe
        log_func("CHILD2 write into second pipe");
        err_control(bytes_written,"CHILD2 write into second pipe: ",0);

        printf("\n   bytes written -->  %ld\n", bytes_written); fflush(stdout);
        if (bytes_written == w_totsize)
        {
                printf("WRITING COMPLETED\n"); fflush(stdout);
        }
        else
                printf("WRITING NOT COMPLETED\n"); fflush(stdout);

        int close_p2write_ret = close(fildes_secondpipe[1]);  // after writing the sorted array in the second pipe close its writing side 
        log_func("CHILD2 close second pipe write-side");
        err_control(close_p2write_ret,"CHILD2 after writing close second pipe write-side: ",0);

        exit(EXIT_SUCCESS);
}
