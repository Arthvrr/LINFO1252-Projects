#ifndef TEST_AND_TEST_AND_SET_PROD_CONS_H
#define TEST_AND_TEST_AND_SET_PROD_CONS_H

#include <stdio.h> //printf()
#include <stdlib.h> //atoi()
#include <pthread.h> //threads()
#include <stdbool.h> //booléens
#include "test_and_test_and_set/test_and_test_and_set_semaphore.h"

#define BUFFER_SIZE 8
#define NUMBER_ELEMENTS 131072
#define MAX_ACTIVE_THREADS 8

extern int buffer[BUFFER_SIZE];
extern int thread_prod;
extern int thread_cons;
extern int produced;
extern int consumed;
extern int idx;

extern int finished;

extern pthread_mutex_t mutex;
extern my_semaphore is_full;
extern my_semaphore is_empty;

void* produce();
void* consume();

#endif //TEST_AND_TEST_AND_SET_PROD_CONS_H