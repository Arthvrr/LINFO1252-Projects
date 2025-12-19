#ifndef TEST_AND_SET_H
#define TEST_AND_SET_H

#include <pthread.h> //threads()
#include <stdio.h> //printf()
#include <unistd.h> //sleep()
#include <stdlib.h> //atoi()

typedef struct verrou {
    volatile int is_locked;
    pthread_mutex_t mutex; //mutex pour my_semaphore
} verrou;

void init_lock(verrou *current_lock);
void lock(verrou *current_lock);
void unlock(verrou *current_lock);
void* thread_func(void* arg);

#endif // TEST_AND_SET_H