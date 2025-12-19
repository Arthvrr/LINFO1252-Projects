#ifndef READER_WRITER_H
#define READER_WRITER_H

#include <pthread.h> //threads()
#include <semaphore.h> //semaphore()
#include <stdio.h> //printf()
#include <unistd.h> //sleep()
#include <stdlib.h> //atoi()
#include <stdbool.h> //booléens

extern int thread_writer;
extern int thread_reader;
extern int number_writed;
extern int number_readed;

extern pthread_mutex_t mutex;
extern sem_t resource_access;
extern int active_readers;

void* write_task(void* arg);
void* read_task(void* arg);

#endif // READER_WRITER_H