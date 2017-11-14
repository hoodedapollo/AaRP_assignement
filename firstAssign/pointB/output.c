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
    
    pid_t wait_pid = wait(NULL);
    printf("\npid from wait -->  %d", wait_pid);
 
    fildes[0] = atoi(argv[1]);
    fildes[1] = atoi(argv[2]);
     
    close(fildes[1]); 
   
    printf("\noutput fildes[0] --> %d", fildes[0]); 
    size_t w_size = sizeof((w[0]));
    printf("\nsize of w[i] -- >  %ld", w_size); 
    printf("\nbytes read: ");
    for (int i = 0; i<10; i++) {
        bytes_read[i] = read(fildes[0], &w[i], w_size);
        perror("read");
        printf(" %ld",bytes_read[i]);
    }
    
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
