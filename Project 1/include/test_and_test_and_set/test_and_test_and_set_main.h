#ifndef TEST_AND_TEST_AND_SET_MAIN_H
#define TEST_AND_TEST_AND_SET_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "test_and_test_and_set/test_and_test_and_set.h"

#define TOTAL_CRITICAL_SECTION 32768

void* test_func(void* arg);
int main();

#endif // TEST_AND_TEST_AND_SET_MAIN_H

