#ifndef PROD_CONS_H
#define PROD_CONS_H

#include <stdio.h> //printf()
#include <stdlib.h> //atoi()
#include <pthread.h> //threads()
#include <semaphore.h> //sémaphores
#include <stdbool.h> //booléens

#define BUFFER_SIZE 8
#define NUMBER_ELEMENTS 131072

extern int buffer[BUFFER_SIZE];
extern int thread_prod;
extern int thread_cons;
extern int produced;
extern int consumed;
extern int idx;

extern pthread_mutex_t mutex;
extern sem_t is_full;
extern sem_t is_empty;

void* produce();
void* consume();

#endif // PROD_CONS_H