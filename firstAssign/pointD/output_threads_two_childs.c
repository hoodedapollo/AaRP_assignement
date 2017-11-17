#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

typedef void (*sighandler_t)(int);

void handler(int signo); // signal handling function 

void err_control(int func_ret, char perror_msg[], int err_value);  // error handling function

void signal_err_control(sighandler_t func_ret, char perror_msg[]);

void bubble_sort(int *a);

int main (int argc, char *argv[]) {
        int w[10];      
        int fildes_firstpipe[2];
        int fildes_secondpipe[2];
        sighandler_t signal_sigusr1_ret, signal_sigusr2_ret;
        ssize_t bytes_read[10];     // array which stores number of bytes read during each reading
        int total_bytes_read;

        signal_sigusr1_ret = signal(SIGUSR1,SIG_IGN);
        signal_err_control(signal_sigusr1_ret,"CHILD2: signal SIGUSR1");
        signal_sigusr2_ret = signal(SIGUSR2, handler); 
        signal_err_control(signal_sigusr2_ret,"CHILD2: signal SIGUSR2");

        fildes_firstpipe[0] = atoi(argv[1]);  // convert string pipes file descriptors pointed by argv into integers
        fildes_firstpipe[1] = atoi(argv[2]);
        fildes_secondpipe[0] = atoi(argv[3]);
        fildes_secondpipe[1] = atoi(argv[4]);

        close(fildes_firstpipe[1]);  // close writing side of the first  pipe before entering the loop
        close(fildes_secondpipe[0]);  // close the reading side of the second pipe before entering the loop

        while (1)
        {

//======= read unsorted vector from first pipe =======================================================================================

                total_bytes_read = 0;  // initialize total_bytes read each time before reading from pipe
                size_t w_size = sizeof((w[0]));
                for (int i = 0; i<10; i++) 
                {
                        bytes_read[i] = read(fildes_firstpipe[0], &w[i], w_size);
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


void handler(int signo) {
        if (signo == SIGUSR2)   
        {
                printf("CHILD2: Signal %d recieved. Terminating child2 process", signo);
                exit(0);
        }
}

void err_control (int func_ret, char perror_msg[], int err_value) {  // error handling function
        if (func_ret < err_value)
        {
                perror(perror_msg);
                exit(EXIT_FAILURE);
        }
}

void signal_err_control(sighandler_t func_ret, char perror_msg[]) {
        if (func_ret == SIG_ERR)
        {
                perror(perror_msg);
                exit(EXIT_FAILURE);
        }
}

void bubble_sort(int *a) {
        for(int j = 0; j<10; j++) 
        {
                for(int i=0;i<9;i++) 
                {
                        if (a[i] > a[i+1]) 
                        {
                                int temp = a[i];
                                a[i] = a[i+1];
                                a[i+1] = temp;
                        }
                }
        }
}
