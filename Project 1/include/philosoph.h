#ifndef PHILOSOPH_H
#define PHILOSOPH_H

#include <stdio.h> //printf()
#include <stdlib.h> //atoi()
#include <pthread.h> //threads()
#include <semaphore.h> //sémaphores
#include <unistd.h> //sleep()
#include <time.h> //mesurer le temps d'exécution

extern int N;
extern sem_t *forks;

void* philosopher(void *arg);
int main(int argc, char *argv[]);

#endif //PHILOSOPH_H