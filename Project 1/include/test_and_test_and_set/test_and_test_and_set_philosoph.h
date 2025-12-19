#ifndef TEST_AND_TEST_AND_SET_PHILOSOPH_H
#define TEST_AND_TEST_AND_SET_PHILOSOPH_H

#include <stdio.h> //printf()
#include <stdlib.h> //atoi()
#include <pthread.h> //threads()
#include <unistd.h> //sleep()
#include <time.h> //mesurer le temps d'exécution
#include "test_and_test_and_set/test_and_test_and_set_semaphore.h"

extern int N; //nombre de philosophes (égal au nombre de fourchettes)
extern my_semaphore *forks; //tableau de fourchettes contenant les sémaphores

void* philosopher(void *arg);
int main(int argc, char *argv[]);

#endif // TEST_AND_TEST_AND_SET_PHILOSOPH_H