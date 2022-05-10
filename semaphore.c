#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

#define CANT_THREADS 5
#define MAX_RENDERS 100


int main(int argc, char *argv[]) {
     
    FILE *fptr;

    fptr = fopen("semaphore_info.txt", "w"); // Write mode

    if(fptr < 0) {
        printf("Error in file");
        return 1;
    }

    printf("Iniciando windoes... a te creas");

    fclose(fptr);

    return 0;
}