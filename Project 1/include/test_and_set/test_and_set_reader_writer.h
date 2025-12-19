#ifndef TEST_AND_SET_READER_WRITER_H
#define TEST_AND_SET_READER_WRITER_H

#include <pthread.h> //threads()
#include <stdio.h> //printf()
#include <unistd.h> //sleep()
#include <stdlib.h> //atoi()
#include <stdbool.h> //booléens
#include "test_and_set/test_and_set_semaphore.h"

void* write_task(void* arg);
void* read_task(void* arg);

extern int thread_writer;
extern int thread_reader;
extern int number_writed;
extern int number_readed;
extern pthread_mutex_t mutex;
extern my_semaphore resource_access;
extern int active_readers;

#endif //TEST_AND_SET_READER_WRITER_H