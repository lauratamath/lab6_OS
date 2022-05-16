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

#define TOTAL_THREADS 8
#define MAX_DEPHT 50

int maxSharing = MAX_DEPHT;

// <------------------------------------------------------
struct myFile {
    FILE* file;
};

// <------------------------------------------------------
struct monitor {
    sem_t ReadytoUse;
    sem_t ReadytoReturn;
    int countResources; 
    int using;
    int requested;
} monitorInfo;
sem_t sem;

// <------------------------------------------------------
void monitorStartup() {
    if(monitorInfo.using || monitorInfo.countResources != 0)
    {
        // usamos wait para que el hilo espere a que el monitor libere los recursos
        sem_wait(&(monitorInfo.ReadytoUse)); 
    }

    monitorInfo.using++;
}

// <------------------------------------------------------
void monitorDeath() {
    // decrementamos el contador de uso
    monitorInfo.using--; 

    // si el contador de uso es 0, entonces podemos liberar los recursos
    if(monitorInfo.requested) 
    {
        // liberamos los recursos
        sem_post(&(monitorInfo.ReadytoReturn));
    } 

    else 
    {
        // liberamos los recursos
        sem_post(&(monitorInfo.ReadytoUse));
    }
}

// <------------------------------------------------------
void monitorAnalyze() 
{
    if(monitorInfo.using) 
    {
        monitorInfo.requested++;
        sem_wait(&(monitorInfo.ReadytoReturn));
        monitorInfo.requested--;
    }

    monitorInfo.countResources++;
    sem_post(&(monitorInfo.ReadytoReturn));
}

void monitorReturningEnding() {
    monitorInfo.countResources--;
    if(monitorInfo.countResources == 0){
        sem_post(&(monitorInfo.ReadytoReturn));
    }
}

int monitorInit() {
    //init struct values
    monitorInfo.countResources = 0;
    monitorInfo.using = 0;
    monitorInfo.requested = 0;

    // init semaphores
    if(sem_init(&(monitorInfo.ReadytoUse), 0, 1) == 0 && sem_init(&(monitorInfo.ReadytoReturn), 0, 1) == 0) {
        return 0;
    } else {
        printf("Error in semaphore initiliazation");
    }
    return 1;

}

void monitorKill() {
    sem_destroy(&(monitorInfo.ReadytoUse));
    sem_destroy(&(monitorInfo.ReadytoReturn));
}

// <------------------------------------------------------
int addResources(int count) 
{
    maxSharing += count;
    return 0;
}

// <------------------------------------------------------
int substractResources(int count) 
{
    if(maxSharing < count) 
    {
        return -1;
    } else {
        maxSharing -= count;
        return 0;
    }
}

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
    int resAvailable = 4;
    fprintf(((struct myFile *) value)->file, "Inicia el thread: %d\n", tid);

    int i = 0;
    for(i; i < MAX_DEPHT; i++)
    {
        fprintf(((struct myFile *) value)->file, "Iters: %d\n", i+1);
        monitorStartup();
        substractResources(resAvailable); //get the resource
        fprintf(((struct myFile *) value)->file, "%d >> Tomando el recurso: \n", tid);
        fprintf(((struct myFile *) value)->file, "%d >> Usando el recurso: \n", tid);
        waitForThreads();
        addResources(resAvailable); //free resource
        fprintf(((struct myFile *) value)->file, "%d >> El recurso usado: \n", tid);
        monitorDeath();
        monitorAnalyze();
        fprintf(((struct myFile *) value)->file, "%d >> El recurso que devuelve es: \n", tid);
        monitorReturningEnding();
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

    FILE *file_container;
    pthread_t thread_ids[TOTAL_THREADS];
    sem_init(&sem, 0, 1); //
    int i = 0;
    int l = 0;

    //para modificar el archivo
    file_container = fopen("monitor.txt", "w"); 

    if(file_container < 0) 
    {
        printf("No se pudo abrir el archivo");
        return 1;
    }

    struct myFile* myFile_container = (struct myFile *) malloc(sizeof(struct myFile));
    myFile_container->file = file_container;

    //inicia la estructura del monitor
    if(monitorInit() == 0) {
        fprintf(file_container, "Initialized monitor\n");
    }
    fprintf(file_container, ">> Inicio: ");
    fprintf(file_container, ">> Creado threads: ");
    
    for(i; i < TOTAL_THREADS; i -= -1)
    {
        pthread_create(&thread_ids[i], NULL, *threadSimulation, (void *)myFile_container);
    }

    for(l; l < TOTAL_THREADS; l -= -1) 
    {
        pthread_join(thread_ids[l], NULL);
    }
    
    // para cerrar el archivo y terminar el programa
    monitorKill();
    fclose(file_container);

    return 0;
}