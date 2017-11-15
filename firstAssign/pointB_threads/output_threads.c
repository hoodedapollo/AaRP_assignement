#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {
        int w[10];      
        int fildes[2];      // file descriptors     
        ssize_t bytes_read[10];     // array which stores number of bytes read during each reading
        int total_bytes_read = 0;
        pid_t wait_ret;
        
        wait_ret = wait(NULL);
        if (wait_ret < 0)
        {
                perror("wait");
                return -1;
        }

        fildes[0] = atoi(argv[1]);
        fildes[1] = atoi(argv[2]);

        printf("\nREADING FROM PIPE"); fflush(stdout);

        close(fildes[1]); 

        size_t w_size = sizeof((w[0]));
        for (int i = 0; i<10; i++) 
        {
                bytes_read[i] = read(fildes[0], &w[i], w_size);
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



                                        close(fildes[0]);   

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

                                        exit(EXIT_SUCCESS);
}
