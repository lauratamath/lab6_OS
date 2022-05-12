// ------------------------------------
// Universidad del Valle de Guatemala
// Sistemas Operativos
// Martín España 
// Laura Tamath
// Diego Álvarez
// Alejandra Gudiel
// ------------------------------------


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

#define TOTAL_THREADS 8
#define MAX_DEPHT 50

struct myFile {
    FILE* file;
};


struct monitor {
    sem_t ReadytoUse;
    sem_t ReadytoReturn;
    int countResources; 
    int using;
    int requested;
} monitor_data;

void *thread_simulation(void *value) {
    for (int i = 0; i < MAX_DEPHT; i++) {
        printf("El thread # %d se esta ejecutando...", *value);
    }
}

int main(int argc, char *argv[]) {
    
    printf("|||      |||  \\\\\\            ///  ||||||||||||");
    printf("|||      |||   \\\\\\          ///   |||");
    printf("|||      |||    \\\\\\        ///    |||");
    printf("|||      |||     \\\\\\      ///     |||  |||||||");
    printf("|||      |||      \\\\\\    ///      |||      |||");
    printf("|||      |||       \\\\\\  ///       |||      |||");
    printf("||||||||||||        \\\\\\///        ||||||||||||");

    FILE *fptr;
    pthread_t thread_id

    fptr = fopen("monitor_info.txt", "w"); // Write mode

    // Check for error opening the file
    if(fptr < 0) {
        printf("There was an error opening the file \"monitor_info.txt\"");
        return 1;
    }

    for (int i = 0; i < TOTAL_THREADS; i++){
        pthread_create(&thread_id, NULL, thread_simulation, (void *)&thread_id);
    }

    // Close the file at the end of the code
    fclose(fptr);

    return 0;
}