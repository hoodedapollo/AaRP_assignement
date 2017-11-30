#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointB_lib.h"

int main (int argc, char *argv[]) {
        int w[10];      
        int fildes[2];      // file descriptors     
        ssize_t bytes_read[10];     // array which stores number of bytes read during each reading
        int total_bytes_read = 0;

        fildes[0] = atoi(argv[1]);  // convert file descriptors into char arrays
        fildes[1] = atoi(argv[2]);

        int close_write_ret = close(fildes[1]); // before reading from pipe close its writing side
        log_func("CHILD2 close pipe write-side");
        err_control(close_write_ret,"CHILD2 close pipe write-side: ",0);


        size_t w_size = sizeof((w[0]));
        for (int i = 0; i<10; i++) 
        {
                bytes_read[i] = read(fildes[0], &w[i], w_size); // read each element of the array from the pipe one after the other and store it in an array
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

        int close_read_ret = close(fildes[0]);  // after reading from the pipe close its reading side 
        log_func("CHILD2 close pipe read-side");
        err_control(close_read_ret,"CHILD2 close pipe read-side: ",0);

        bubble_sort(w);  // sort the number in the array

        vector_print(w,"The ordered vector is"); // print to stdout the sorted array of numbers

        exit(EXIT_SUCCESS);
}
