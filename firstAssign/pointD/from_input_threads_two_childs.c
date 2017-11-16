#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>


void handler(int signo) {
        if (signo == SIGUSR1)
        {
        printf("CHILD1: Signal %d recieved. Terminating child1 process\n", signo); fflush(stdout);
        exit(EXIT_SUCCESS);
        }
}    

int main(int argc, char *argv[]) {
        int v[10],w[10];
        int fildes_firstpipe[2];
        int fildes_secondpipe[2];
        int scanf_ret;
        ssize_t bytes_read[10];     // array which stores number of bytes read during each reading
        int total_bytes_read;
        char choice;
        int pid_child2;
        signal(SIGUSR1,handler);
        signal(SIGUSR2,SIG_IGN);
        fildes_firstpipe[0] = atoi(argv[1]);
        fildes_firstpipe[1] = atoi(argv[2]); 

        fildes_secondpipe[0] = atoi(argv[3]);
        fildes_secondpipe[1] = atoi(argv[4]); 

        while(1)
        {
                while (choice != 'y' &&  choice != 'n')
                {
                        printf("Do you want to enter a new vector to be sorted?(y/n)\n"); fflush(stdout);
                        scanf(" %c", &choice);
                }
                if (choice == 'n')
                {
                        printf("All the process are going to be terminated\n"); fflush(stdout);
                        sleep(2); // prima di uccidere l'niverso aspetta 5 secondi
                        kill(0,SIGUSR2);
                        sleep(2);
                        raise(SIGUSR1);
                }
                else
                {
                        choice = 'a';
                        printf("PLEASE INSERT 10 NUMBERS");  fflush(stdout); 
                        for(int i=0;i<10;i++) 
                        {
                                printf("\n   insert the %d-th number --> ",i+1);  fflush(stdout);  
                                scanf_ret = scanf("%d", &v[i]);
                                if (scanf_ret < 0)
                                {
                                        perror("scanf");
                                        return -1;
                                }

                        }
                        printf("\nWELL DONE\nYou have inserted the following numbers:\n\n["); fflush(stdout);

                        for(int i=0;i<10;i++) {
                                printf(" %d", v[i]); fflush(stdout);
                        }
                        printf(" ]\n\n"); fflush(stdout);

                        close(fildes_firstpipe[0]); 

                        size_t v_size = 10 * sizeof(v[0]);
                        printf("\nCHILD1: WRITING IN THE PIPE...\n   size of vector v -->  %ld",v_size); fflush(stdout);
                        ssize_t bytes_written = write(fildes_firstpipe[1], v, v_size);
                        if (bytes_written < 0)
                        {
                                perror("write"); 
                                return -1;
                        }
                        printf("\n   bytes written -->  %ld\n", bytes_written); fflush(stdout);
                        if (bytes_written == v_size)
                        {
                                printf("CHILD1: WRITING COMPLETED\n"); fflush(stdout);
                        }
                        else 
                                printf("CHILD1: WRITING NOT COMPLETED\n"); fflush(stdout);


                        close(fildes_secondpipe[1]); // before reading close writing side of the second pipe
                        total_bytes_read = 0;
                        size_t w_size = sizeof((w[0]));
                        for (int i = 0; i<10; i++)
                        {
                                bytes_read[i] = read(fildes_secondpipe[0], &w[i], w_size);
                                total_bytes_read = total_bytes_read + bytes_read[i];
                                if (bytes_read[i] < 0)
                                {
                                        perror("read");
                                        return -1;
                                }
                                else if (bytes_read[i] < w_size)
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
                                printf("CHILD1: READING COMPLETED\n"); fflush(stdout);

                        printf("\nThe ordered vector is\n[");

                        for (int i=0;i<10;i++) 
                        {
                                printf(" %d", w[i]);
                        }
                        printf(" ]\n");


                }
        }
}    

