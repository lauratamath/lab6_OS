#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

#define CANT_THREADS 5
#define MAX_RENDERS 100

struct myFile {
    FILE* file;
};


struct monitor {
    sem_t ReadytoUse;
    sem_t ReadytoReturn;
    int countResources; 
    int using;
    int requested;
};

struct monitor monitor_data;

int main(int argc, char *argv[]) {
    
    FILE *fptr;

    fptr = fopen("monitor_info.txt", "w"); // Write mode

    if(fptr < 0) {
        printf("Error in file");
        return 1;
    }

    printf("Iniciando windoes... a te creas");

    fclose(fptr);

    return 0;
}