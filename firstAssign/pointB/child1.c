#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointB_lib.h"

int main(int argc, char *argv[]) {
        int v[10];
        int fildes[2];
        int scanf_ret;

        fildes[0] = atoi(argv[1]);  // convert file descriptors into char arrays
        fildes[1] = atoi(argv[2]); 
        printf("PLEASE INSERT 10 NUMBERS");  fflush(stdout); 
        for(int i=0;i<10;i++) // store the numbers inserted by keyboard into an array 
        {
                printf("\n   insert the %d-th number --> ",i+1);  fflush(stdout);  
                scanf_ret = scanf("%d", &v[i]);
                log_func("CHILD1 i-th scanf");
                err_control(scanf_ret,"CHILD1 i-th scanf: ",0);
        }

        vector_print(v,"\nWELL DONE\nYou have inserted the following numbers:"); // print the vector to stdout

        int close_read_ret = close(fildes[0]); // before writing into the pipe close its read side
        log_func("CHILD1 close pipe read-side");
        err_control(close_read_ret,"CHILD1 close pipe read-side: ",0);

        size_t v_size = 10 * sizeof(v[0]);
        printf("\nWRITING IN THE PIPE...\n   size of vector v -->  %ld",v_size); fflush(stdout);
        ssize_t bytes_written = write(fildes[1], v, v_size); // writing the array in the pipe
        log_func("CHILD1 write into pipe");
        err_control(bytes_written,"CHILD1 write into pipe: ",0);

        printf("\n   bytes written -->  %ld\n", bytes_written); fflush(stdout);
        if (bytes_written == v_size)
        {
                printf("WRITING COMPLETED\n"); fflush(stdout);
        }
        else
        { 
                printf("WRITING NOT COMPLETED\n"); fflush(stdout);
        }

        int close_write_ret = close(fildes[1]); // after writing close the writing side of the pipe
        log_func("CHILD1 close pipe write-side");
        err_control(close_write_ret,"CHILD1 after writing close pipe write-side: ",0);

        exit(EXIT_SUCCESS);
}    

