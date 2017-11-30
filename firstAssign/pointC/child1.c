#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointC_lib.h"

int main(int argc, char *argv[]) {
        int v[10],w[10];
        int fildes_firstpipe[2];
        int fildes_secondpipe[2];
        int scanf_ret;
        ssize_t bytes_read[10];     // array which stores number of bytes read during each reading
        int total_bytes_read = 0;

        fildes_firstpipe[0] = atoi(argv[1]);
        fildes_firstpipe[1] = atoi(argv[2]); 

        fildes_secondpipe[0] = atoi(argv[3]);
        fildes_secondpipe[1] = atoi(argv[4]); 

        printf("PLEASE INSERT 10 NUMBERS");  fflush(stdout); 
        for(int i=0;i<10;i++) 
        {
                printf("\n   insert the %d-th number --> ",i+1);  fflush(stdout);  
                scanf_ret = scanf("%d", &v[i]);
                log_func("CHILD1 i-th scanf");
                err_control(scanf_ret,"CHILD1 i-th scanf: ",0);

        }

        vector_print(v,"\nWELL DONE\nYou have inserted the following numbers:");

        int close_p1read_ret = close(fildes_firstpipe[0]); 
        log_func("CHILD1 close first pipe read-side");
        err_control(close_p1read_ret,"CHILD1 close first pipe read-side: ",0); 
        size_t v_size = 10 * sizeof(v[0]);
        printf("\nWRITING IN THE PIPE...\n   size of vector v -->  %ld",v_size); fflush(stdout);
        ssize_t bytes_written = write(fildes_firstpipe[1], v, v_size);
        log_func("CHILD1 write into first pipe");
        err_control(bytes_written,"CHILD1 write into first pipe: ",0);
        printf("\n   bytes written -->  %ld\n", bytes_written); fflush(stdout);
        if (bytes_written == v_size)
        {
                printf("WRITING COMPLETED\n"); fflush(stdout);
        }
        else 
                printf("WRITING NOT COMPLETED\n"); fflush(stdout);

        int close_p1write_ret = close(fildes_firstpipe[1]); // after writing close the writing side of the first pipe   
        log_func("CHILD1 close first pipe write-side");
        err_control(close_p1write_ret,"CHILD1 after writing close first pipe write-side: ",0);

        int close_p2write_ret = close(fildes_secondpipe[1]); // before reading close writing side of the second pipe
        log_func("CHILD1 close second pipe write-side");
        err_control(close_p2write_ret,"CHILD2 close second pipe write-side: ",0);

        size_t w_size = sizeof((w[0]));
        for (int i = 0; i<10; i++)
        {
                bytes_read[i] = read(fildes_secondpipe[0], &w[i], w_size);
                log_func("CHILD1 i-th read");
                err_control(bytes_read[i],"CHILD1 i-th read: ",0);
                total_bytes_read = total_bytes_read + bytes_read[i];
                if (bytes_read[i] < w_size)
                {
                        printf("\nPARTIAL READ: bytes read by the i-th read -->  %ld\n",bytes_read[i]);
                }
        }

        int close_p2read_ret = close(fildes_secondpipe[0]); // after reading close the reading side of the pipe
        log_func("CHILD1 close second pipe read-side");
        err_control(close_p2read_ret,"CHILD1 after reading close second pipe read-side: ",0);
        printf("\n   Total number of bytes read --> %d\n", total_bytes_read); fflush(stdout);

        if (total_bytes_read < 10 * w_size)
        {
                printf("PARTIAL READING\n"); fflush(stdout);
        }
        else
        {
                printf("READING COMPLETED\n"); fflush(stdout);
        }
        
        vector_print(w,"The ordered vector is:");

        exit(EXIT_SUCCESS);

}    

