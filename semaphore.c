// ------------------------------------
// Universidad del Valle de Guatemala
// Sistemas Operativos: Laboratorio 6
// Martín España    19258
// Laura Tamath     19365
// Diego Álvarez    19498
// Alejandra Gudiel 19232
// ------------------------------------

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>

// <------------------------------------------------------
#define TOTAL_THREADS 8
#define MAX_DEPHT 50
int maxSharing = MAX_DEPHT;
sem_t sem;

// <------------------------------------------------------
struct myFile {
    FILE* file;
};

// <------------------------------------------------------
void waitForThreads() 
{
    time_t t;
    srand((unsigned) time(&t));
    int waitingTime = rand() % 2;
    sleep(waitingTime);
}

// <------------------------------------------------------
void *threadSimulation(void *value) 
{
    int tid = syscall(SYS_gettid);
    
    fprintf(((struct myFile *) value)->file, "Inicia el thread: %d\n", tid);

    int i;
    for(i=0; i< MAX_DEPHT; i++){
        fprintf(((struct myFile *) value)->file, "Primera iteración %d\n", i+1);
        sem_wait(&sem);

        // get al recurso
        maxSharing--;
        fprintf(((struct myFile *) value)->file, "%d >> Tomando el recurso: \n", tid);
        fprintf(((struct myFile *) value)->file, "%d >> Usando el recurso: \n", tid);
        waitForThreads();
        // release al recurso
        maxSharing++;

        fprintf(((struct myFile *) value)->file, "%d >> El recurso usado: \n", tid);
        
        sem_post(&sem); 
        
        fprintf(((struct myFile *) value)->file, "%d >> El recurso que devuelve es: \n", tid);
        
    }
        
    fprintf(((struct myFile *) value)->file, "%d >> Terminando el uso del semáforo \n", tid);
    fprintf(((struct myFile *) value)->file, "%d >> Terminando el proceso", tid);

    pthread_exit(0);
}

// <------------------------------------------------------
int main(int argc, char *argv[])
{
    
    printf("|||      |||  \\\\\\            ///  ||||||||||||\n");
    printf("|||      |||   \\\\\\          ///   |||\n");
    printf("|||      |||    \\\\\\        ///    |||\n");
    printf("|||      |||     \\\\\\      ///     |||  |||||||\n");
    printf("|||      |||      \\\\\\    ///      |||      |||\n");
    printf("|||      |||       \\\\\\  ///       |||      |||\n");
    printf("||||||||||||        \\\\\\///        ||||||||||||\n");

    FILE *fileContainer;
    pthread_t thread_ids[TOTAL_THREADS];
    sem_init(&sem, 0, 1);

    //para modificar el archivo
    fileContainer = fopen("semaphore.txt", "w"); 

    if(fileContainer < 0) 
    {
        printf("No se pudo abrir el archivo");
        return 1;
    }

    struct myFile* myFileContainer = (struct myFile *) malloc(sizeof(struct myFile));
    myFileContainer->file = fileContainer;

    fprintf(fileContainer, ">> Inicio: ");
    fprintf(fileContainer, ">> Creado threads: ");
    
    //crea los thread
    sem_init(&sem, 0, 1);

    int i;
    for(i = 0; i < TOTAL_THREADS; i -= -1)
    {
        pthread_create(&thread_ids[i], NULL, *threadSimulation, (void *)myFileContainer);
    }

    int l;
    for(l=0; l<TOTAL_THREADS; l -= -1) 
    {
        pthread_join(thread_ids[l], NULL);
    }
    
    // para cerrar el archivo y terminar el programa
    fclose(fileContainer);
    sem_destroy(&sem);

    return 0;
}