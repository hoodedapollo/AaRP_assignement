#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include "pointD_lib.h"

int main(int argc, char *argv[]) {
        int v[10],w[10];
        int fildes_firstpipe[2], fildes_secondpipe[2];
        int num_scanf_ret;
        int choice_scanf_ret;
        int kill_ret;
        int raise_ret;
        sighandler_t signal_sigusr1_ret, signal_sigusr2_ret;
        ssize_t bytes_read[10];  // array which stores number of bytes read during each reading
        int total_bytes_read;  // total bytes read after reading process is completed
        char choice;

        signal_sigusr1_ret = signal(SIGUSR1,handler);
        log_func("CHILD1 signal SIGUSR1 recieved");
        signal_err_control(signal_sigusr1_ret,"CHILD1: signal SIGUSR1");
        signal_sigusr2_ret = signal(SIGUSR2,SIG_IGN);
        log_func("CHILD1 signal SIGUSR2 recieved");
        signal_err_control(signal_sigusr2_ret,"CHILD1: signal SIGUSR2");

        fildes_firstpipe[0] = atoi(argv[1]); // convert string pipes file descriptors pointed by argv into integers
        fildes_firstpipe[1] = atoi(argv[2]); 
        fildes_secondpipe[0] = atoi(argv[3]);
        fildes_secondpipe[1] = atoi(argv[4]); 

        close(fildes_firstpipe[0]);  // close reading side of the first pipe befor entering the loop
        log_func("CHILD1 close first pipe reading side");
        close(fildes_secondpipe[1]);  // close writing side of the second pipe before entering the loop
        log_func("CHILD1 close second pipe writing  side");

        while(1)
        {
                while (choice != 'y' &&  choice != 'n')  // keep looping until y or n are inserted from keyboard
                {
                        printf("Do you want to enter a new vector to be sorted?(y/n)\n"); fflush(stdout);
                        choice_scanf_ret = scanf(" %c", &choice);
                        log_func("CHILD1 choice scanf");
                        err_control(choice_scanf_ret,"choice scanf",0);
                }
                if (choice == 'n')  // terminates child2 process and than the current process
                {
                        printf("All the process are going to be terminated\n"); fflush(stdout);
                        sleep(2);  // it makes the output easier to read
                        kill_ret = kill(0,SIGUSR2);  // send SIGUSR2 signal to all the processes within child1 process group
                        log_func("CHILD1 kill SIGUSR2");
                        err_control(kill_ret,"kill SIGUSR2",0);
                        sleep(2);  // it makes the output easier to read
                        raise_ret = raise(SIGUSR1);  // send SIGUSER1 to the process itslef
                        err_control(raise_ret,"raise",0);
                }
                else  // run the client
                {
                        choice = 'a';  // reset the choice variable in order to get back inside the first while loop
                        printf("PLEASE INSERT 10 NUMBERS");  fflush(stdout); 
                        for(int i=0;i<10;i++)  // read ten numbers from stdin and store them in a vector
                        {
                                printf("\n   insert the %d-th number --> ",i+1);  fflush(stdout);  
                                num_scanf_ret = scanf("%d", &v[i]);
                                log_func("CHILD1 num i-th scanf");
                                err_control(num_scanf_ret,"number scanf",0);
                        }
                        printf("\nWELL DONE\nYou have inserted the following numbers:\n\n"); fflush(stdout);
                        
                        vector_print(v);

//=============== write unsorted vector into first pipe ================================================================

                        size_t v_size = 10 * sizeof(v[0]);
                        printf("\nCHILD1: WRITING IN THE PIPE...\n   size of vector v -->  %ld",v_size); fflush(stdout);
                        ssize_t bytes_written = write(fildes_firstpipe[1], v, v_size);
                        log_func("CHILD1 pipe1 write");
                        err_control(bytes_written,"CHILD1: first pipe write",0);

                        printf("\n   bytes written -->  %ld\n", bytes_written); fflush(stdout);
                        if (bytes_written == v_size)  // all the data has been written
                        {
                                printf("CHILD1: WRITING COMPLETED\n"); fflush(stdout);
                        }
                        else 
                        {
                                printf("CHILD1: WRITING NOT COMPLETED\n"); fflush(stdout);
                        }

//=============== read sorted vector from second pipe =================================================================                    

                        total_bytes_read = 0; // initialize total_bytes_read each time before reading from pipe
                        size_t w_size = sizeof((w[0]));
                        for (int i = 0; i<10; i++)
                        {
                                bytes_read[i] = read(fildes_secondpipe[0], &w[i], w_size);
                                log_func("CHILD1 pipe1 i-th read");
                                err_control(bytes_read[i],"CHILD1: second pipe read",0);
                                total_bytes_read = total_bytes_read + bytes_read[i];
                                if (bytes_read[i] < w_size)
                                {
                                        printf("\nCHILD1: PARTIAL READ: bytes read by the i-th read -->  %ld\n",bytes_read[i]);
                                }
                        }


                        printf("\n   Total number of bytes read --> %d\n", total_bytes_read); fflush(stdout);

                        if (total_bytes_read < 10 * w_size)
                        {
                                printf("CHILD1: PARTIAL READING\n"); fflush(stdout);
                        }
                        else
                        {
                                printf("CHILD1: READING COMPLETED\n"); fflush(stdout);
                        }

//================ end of reading =====================================================================================

                        vector_print(w); // print to stdout the sorted vector

                }
        }
}    

