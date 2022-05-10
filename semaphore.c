#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

#define CANT_THREADS 8
#define MAX_DEPHT 50

// ------------------------------------
// Universidad del Valle de Guatemala
// Sistemas Operativos
// Martín España 
// Laura Tamath
// Diego Álvarez
// Alejandra Gudiel
// ------------------------------------

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

    fptr = fopen("semaphore_info.txt", "w"); // Write mode

    if(fptr < 0) {
        printf("Error in file");
        return 1;
    }

    for (int i = 0; i < TOTAL_THREADS; i++){
        pthread_create(&thread_id, NULL, thread_simulation, (void *)&thread_id);
    }
    
    fclose(fptr);

    return 0;
}