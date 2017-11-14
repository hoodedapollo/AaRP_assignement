#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int v[10];
    char c[10][20];
    int fildes[2];
    int pid_child1;
printf("dbg1\n"); fflush(stdout);
    fildes[0] = atoi(argv[1]);
    fildes[1] = atoi(argv[2]); 
    pid_child1 = atoi(argv[3]);    
printf("dbg2\n"); fflush(stdout);
    printf("PLEASE INSERT 10 NUMBERS"); fflush(stdout); 
    getchar();
    for(int i=0;i<10;i++) 
    {
        printf("\n   insert the %d-th number --> ",i+1); fflush(stdout);  
        fgets(c[i],100,stdin);
        v[i] = atoi(c[i]);
        
    }
    printf("\nWELL DONE\nYou have inserted the following numbers:\n\n["); fflush(stdout);

    for(int i=0;i<10;i++) {
    printf(" %d", v[i]); fflush(stdout);
    }
    printf(" ]\n\n"); fflush(stdout);

    close(fildes[0]); 
    
    size_t v_size = 10 * sizeof(v[0]);
    printf("\nWRITING IN THE PIPE...\n   size of vector v -->  %ld",v_size); fflush(stdout);
    ssize_t bytes_written = write(fildes[1], v, v_size);
    if (bytes_written < 0)
    {
        perror("write"); 
        return -1;
    }
    printf("\n   bytes written -->  %ld\n", bytes_written); fflush(stdout);
    if (bytes_written == v_size)
    {
        printf("WRITING COMPLETED\n"); fflush(stdout);
    }
    else 
        printf("WRITING NOT COMPLETED\n"); fflush(stdout);

    close(fildes[1]);    
    
    kill(pid_child1, SIGUSR1);

    exit(EXIT_SUCCESS);
    
}    
    
