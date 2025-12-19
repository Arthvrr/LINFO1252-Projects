#ifndef TEST_AND_TEST_AND_SET_SEMAPHORE_H
#define TEST_AND_TEST_AND_SET_SEMAPHORE_H

#include <pthread.h> //threads()
#include <stdio.h> //printf()
#include <unistd.h> //sleep()
#include <stdlib.h> //atoi()
#include "test_and_test_and_set.h" //fonctions du 2.3

typedef struct my_semaphore {
    int count; //nombre de ressources disponibles
    verrou lock; //verrou pour protégér l'accès
    pthread_cond_t condition; //condition pour suspendre ou réveiller les threads
} my_semaphore;

void my_semaphore_init(my_semaphore *sem, int initial_count);
void my_semaphore_wait(my_semaphore *sem);
void my_semaphore_signal(my_semaphore *sem);
void my_semaphore_destroy(my_semaphore *sem);

#endif // TEST_AND_TEST_AND_SET_SEMAPHORE_H