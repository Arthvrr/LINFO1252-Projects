#include "reader_writer.h"


// 1.3 Problèmes des lecteurs & écrivains


#define TOTAL_WRITE 640
#define TOTAL_READ 2540

int thread_writer;
int thread_reader;
int number_writed = 0;
int number_readed = 0;

pthread_mutex_t mutex;
sem_t resource_access;
int active_readers = 0;


void* write_task(void* arg){

    //printf("Writer thread started. Native ID: %lu\n", pthread_self());

    while (true) {

        pthread_mutex_lock(&mutex);

        if (number_writed >= TOTAL_WRITE) { //toutes les écritures sont faites
            pthread_mutex_unlock(&mutex);
            break;
        }

        number_writed++;
        pthread_mutex_unlock(&mutex);

        sem_wait(&resource_access);

        //écriture dans la section critique
        for (int i = 0; i < 10000; i++);

        sem_post(&resource_access); //libère la ressource
    }
    //printf("Writer thread finished. Native ID: %lu\n", pthread_self());
    return NULL;
}

void* read_task(void* arg){

    //printf("Reader thread started. Native ID: %lu\n", pthread_self());

    while (true) {

        pthread_mutex_lock(&mutex);

        if (number_readed >= TOTAL_READ) { //toutes les lectures sont faites
            pthread_mutex_unlock(&mutex);
            break;
        }

        number_readed++;
        pthread_mutex_unlock(&mutex);

        pthread_mutex_lock(&mutex);
        active_readers++;
        if (active_readers == 1) {
            sem_wait(&resource_access); //on bloque l'accès aux écrivains (prioritaires)
        }
        pthread_mutex_unlock(&mutex);

        //lecture dans la section critique
        for (int i = 0; i < 10000; i++);

        pthread_mutex_lock(&mutex);
        active_readers--;
        if (active_readers == 0) {
            sem_post(&resource_access); //on libère l'accès
        }
        pthread_mutex_unlock(&mutex);
    }
    //printf("Reader thread finished. Native ID: %lu\n", pthread_self());
    return NULL;
}


int main(int argc, char *argv[]){

    if (argc != 3) {
        printf("Please give us the number of Writer and/or Reader threads !\n");
        return 1;
    }

    thread_writer = atoi(argv[1]);
    thread_reader = atoi(argv[2]);

    if (thread_writer <= 0) {
        printf("Number of Writer's thread should be positive !\n");
        return 1;
    }

    if (thread_reader <= 0) {
        printf("Number of Reader's thread should be positive !\n");
        return 1;
    }

    pthread_mutex_init(&mutex, NULL);
    sem_init(&resource_access, 0, 1);

    pthread_t writers[thread_writer];
    pthread_t readers[thread_reader];

    //on crée les threads writer et reader
    for (int i = 0; i < thread_writer; i++) {
        pthread_create(&writers[i], NULL, write_task, NULL);
    }

    for (int i = 0; i < thread_reader; i++) {
        pthread_create(&readers[i], NULL, read_task, NULL);
    }

    //on attends leur fin
    for (int i = 0; i < thread_writer; i++) {
        pthread_join(writers[i], NULL);
    }

    for (int i = 0; i < thread_reader; i++) {
        pthread_join(readers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&resource_access);

    //printf("All tasks are completed. Total writes: %d, Total reads: %d\n", number_writed, number_readed);
    
    return 0;
}