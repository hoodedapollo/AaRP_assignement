#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int v[10];
    int fildes[2];

    fildes[0] = atoi(argv[1]);
    fildes[1] = atoi(argv[2]); 
    
    printf("Please insert 10 numbers");
    for(int i=0;i<10;i++) {
    printf("\ninsert the %d-th number -->  ",i+1);    
    scanf("%d", &v[i]);
    }
    printf("\nWell done\nYou have inserted the following numbers\n[");
    
    for(int i=0;i<10;i++) {
    printf(" %d", v[i]);
    }
    printf(" ]\n\n");
    
    close(fildes[0]); 
    
    printf("\nfrom_input fildes[1] --> %d", fildes[1]);
    size_t v_size = 10 * sizeof(v[0]);
    printf("\nsize of vector v -->  %ld",v_size);
    ssize_t bytes_written = write(fildes[1], v, v_size);
    perror("write");
    printf("\nbytes written -->  %ld", bytes_written);
    close(fildes[1]);    
    
    exit(EXIT_SUCCESS);
    
}    
    
