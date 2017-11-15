#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
        int w[10];      
        int fildes_firstpipe[2];
        int fildes_secondpipe[2];
        ssize_t bytes_read[10];     // array which stores number of bytes read during each reading
        int total_bytes_read = 0;
        
        fildes_firstpipe[0] = atoi(argv[1]);
        fildes_firstpipe[1] = atoi(argv[2]);

        fildes_secondpipe[0] = atoi(argv[3]);
        fildes_secondpipe[1] = atoi(argv[4]);

        close(fildes_firstpipe[1]); 

        size_t w_size = sizeof((w[0]));
        for (int i = 0; i<10; i++) 
        {
                bytes_read[i] = read(fildes_firstpipe[0], &w[i], w_size);
                total_bytes_read = total_bytes_read + bytes_read[i];
                if (bytes_read[i] < 0)
                {
                        perror("read");
                        return -1;
                }
                else if (bytes_read[i] < w_size)
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



        close(fildes_firstpipe[0]);   

        for(int j = 0; j<10; j++) {
                for(int i=0;i<9;i++) {
                        if (w[i] > w[i+1]) {
                                int temp = w[i];
                                w[i] = w[i+1];
                                w[i+1] = temp;
                        }
                }
        }
        printf("\nThe ordered vector is\n[");

        for (int i=0;i<10;i++) { 
                printf(" %d", w[i]);
        }
        printf(" ]\n");
        
        close(fildes_secondpipe[0]); // before writing close the reading side of the second pipe
        
    size_t w_totsize = sizeof(w);
    printf("\nWRITING IN THE PIPE...\n   size of vector v -->  %ld",w_totsize); fflush(stdout);
    ssize_t bytes_written = write(fildes_secondpipe[1], w, w_totsize);
    if (bytes_written < 0)
    {
        perror("write");
        return -1;
    }
    printf("\n   bytes written -->  %ld\n", bytes_written); fflush(stdout);
    if (bytes_written == w_totsize)
    {
        printf("WRITING COMPLETED\n"); fflush(stdout);
    }
    else
        printf("WRITING NOT COMPLETED\n"); fflush(stdout);

    close(fildes_secondpipe[1]);

        exit(EXIT_SUCCESS);
}
