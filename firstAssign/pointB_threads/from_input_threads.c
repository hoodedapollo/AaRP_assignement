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
    int scanf_ret;

    fildes[0] = atoi(argv[1]);
    fildes[1] = atoi(argv[2]); 
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
    
    exit(EXIT_SUCCESS);

}    
    
