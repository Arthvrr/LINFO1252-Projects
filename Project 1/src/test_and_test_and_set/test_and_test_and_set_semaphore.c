#include "test_and_test_and_set/test_and_test_and_set_semaphore.h"


//2.4 Conception d'une interface sémaphore sur base de test_and_test_and_set


//sem_init()
void my_semaphore_init(my_semaphore *sem, int initial_count) {
    sem->count = initial_count; //initialiser le compteur avec le nombre de ressources
    init_lock(&sem->lock); //initialise le verrou, mutex et condition
    pthread_mutex_init(&sem->lock.mutex, NULL);
    pthread_cond_init(&sem->condition, NULL);
}

//sem_wait()
void my_semaphore_wait(my_semaphore *sem) {
    pthread_mutex_lock(&sem->lock.mutex);
    while (sem->count <= 0) { //tant qu'il y a aucune ressource disponible
        pthread_cond_wait(&sem->condition, &sem->lock.mutex); //attendre un signal
    }
    sem->count--; //on réserve une ressource
    pthread_mutex_unlock(&sem->lock.mutex);
}

//sem_post()
void my_semaphore_signal(my_semaphore *sem) {
    pthread_mutex_lock(&sem->lock.mutex);
    sem->count++;
    pthread_cond_signal(&sem->condition); //réveille un thread en attente
    pthread_mutex_unlock(&sem->lock.mutex);
}

//sem_destroy()
void my_semaphore_destroy(my_semaphore *sem) {
    pthread_mutex_destroy(&sem->lock.mutex); //on détruit le mutex et la condition
    pthread_cond_destroy(&sem->condition);
}