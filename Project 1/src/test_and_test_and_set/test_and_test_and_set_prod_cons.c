#include "test_and_test_and_set/test_and_test_and_set_prod_cons.h"


//2.5 Adaptation du 1.1 avec nos primitives d'attentes actives --> problème des producteurs et consommateurs


#define BUFFER_SIZE 8
#define NUMBER_ELEMENTS 131072
#define MAX_ACTIVE_THREADS 8  //limiter à 8 threads producteurs et 8 threads consommateurs actifs


int buffer[BUFFER_SIZE]; //tableau d'entiers buffer
int thread_prod; //nombre de threads du producer
int thread_cons; //nombre de threads du consumer
int produced = 0; //nombre d'éléments produits
int consumed = 0; //nombre d'éléments consommés
int idx = 0; //index dans le buffer

int finished = 0;

pthread_mutex_t mutex;
my_semaphore is_full;
my_semaphore is_empty;

void* produce(){
    /*
    Produit des éléments dans le buffer
    Remplit le buffer --> attend donc quand le buffer est plein
    */
    pthread_t thread_id = pthread_self(); //récupérer l'id du thread
    while (true){
        
        for (int i = 0; i < 1000;i++){} //simulation d'un traitement

        my_semaphore_wait(&is_empty); //on attend une zone libre dans le buffer pour produire

        //SECTION CRITIQUE
        pthread_mutex_lock(&mutex);

        if(produced >= NUMBER_ELEMENTS){ //si le nombre d'éléments produits est plus grand ou égal au nombre d'éléments, on break
            //printf("Producer thread (ID: %ld) finished. Total produced: %d\n", (long)thread_id, produced); // Ajout de printf ici
            finished = 1;

            //libérer les threads consommateurs restants
            for (int i = 0; i < thread_cons; i++) {
                my_semaphore_signal(&is_full);
            }

            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL); //fin du thread
        }

        buffer[idx] = (int) thread_id; //cast vers int
        produced++;
        idx = (idx + 1) % BUFFER_SIZE;

        my_semaphore_signal(&is_full); //+1 place remplie dans le buffer
        pthread_mutex_unlock(&mutex);
    }
}

void* consume(){
    /*
    Consomme les éléments dans le buffer
    Vide le buffer --> attend donc quand le buffer est vide
    */
    while (true){

        for (int j = 0; j < 1000;j++); //simulation d'un traitement

        //si la production est terminée et qu'il n'y a plus rien à consommer
        if (finished && produced == consumed) {
            //printf("Consumer thread %ld finished. Total consumed: %d\n", pthread_self(), consumed);
            pthread_exit(NULL);
        }
        
        my_semaphore_wait(&is_full); //on attend une donnée disponible dans le buffer pour la consommer

        //SECTION CRITIQUE
        pthread_mutex_lock(&mutex);

        if(consumed >= NUMBER_ELEMENTS){ //si le nombre d'éléments consommés est plus grand ou égal au nombre d'éléments, on break
            //printf("Consumer thread (ID: %ld) finished. Total consumed: %d\n", (long)pthread_self(), consumed);
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL); //fin du thread
        }

        int consumed_item = buffer[idx];
        consumed++;
        idx = (idx + 1) % BUFFER_SIZE;

        my_semaphore_signal(&is_empty); //+1 place vide dans le buffer
        pthread_mutex_unlock(&mutex);

    }
}

//tâche vide pour les threads supplémentaires
void* idle(void* arg) {
    pthread_exit(NULL); //fin immédiate des threads inactifs
}


/*
argc = nombre d'arguments
argv = liste des arguments
*/
int main(int argc, char *argv[]) {

    if (argc != 3) {
        //printf("Please give us the number of Producer and/or Consumer threads !\n");
        return 1;
    }

    thread_prod = atoi(argv[1]); //on convertit de *char en int avec atoi()
    thread_cons = atoi(argv[2]);

    if (thread_prod <= 0) {
        //printf("Number of Producer's thread should be positive !\n");
        return 1;
    }

    if (thread_cons <= 0) {
        //printf("Number of Consumer's thread should be positive !\n");
        return 1;
    }

    //printf("Nombre de threads du Producer : %d\n", thread_prod);
    //printf("Nombre de threads du Consumer : %d\n", thread_cons);

    //intialiser le mutex et les sémaphores
    pthread_mutex_init(&mutex, NULL);
    my_semaphore_init(&is_full, 0);            
    my_semaphore_init(&is_empty, BUFFER_SIZE);

    pthread_t producers[thread_prod];
    pthread_t consumers[thread_cons];

    //créer les threads producteurs (actifs ET inactifs)
    for (int i = 0; i < thread_prod; i++) {
        if (i < MAX_ACTIVE_THREADS) {
            if (pthread_create(&producers[i], NULL, produce, NULL) != 0) {
                perror("Failed to create producer thread");
                return 1;
            }
        } else {
            //on crée les threads supplémentaires inactifs
            if (pthread_create(&producers[i], NULL, idle, NULL) != 0) {
                perror("Failed to create idle producer thread");
                return 1;
            }
        }
    }

    //créer les threads consommateurs (actifs ET inactifs)
    for (int i = 0; i < thread_cons; i++) {
        if (i < MAX_ACTIVE_THREADS) {
            if (pthread_create(&consumers[i], NULL, consume, NULL) != 0) {
                perror("Failed to create consumer thread");
                return 1;
            }
        } else {
            //on crée les threads supplémentaires inactifs
            if (pthread_create(&consumers[i], NULL, idle, NULL) != 0) {
                perror("Failed to create idle consumer thread");
                return 1;
            }
        }
    }

    //limiter à 8 threads producteurs actifs ou à moins si moins de threads sont fournis
    for (int i = 0; i < (thread_prod < MAX_ACTIVE_THREADS ? thread_prod : MAX_ACTIVE_THREADS); i++) {
        pthread_join(producers[i], NULL);
    }

    //limiter à 8 threads consommateurs actifs ou à moins si moins de threads sont fournis
    for (int i = 0; i < (thread_cons < MAX_ACTIVE_THREADS ? thread_cons : MAX_ACTIVE_THREADS); i++) {
        pthread_join(consumers[i], NULL);
    }

    //attendre la fin des threads inactifs
    for (int i = MAX_ACTIVE_THREADS; i < thread_prod; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = MAX_ACTIVE_THREADS; i < thread_cons; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    my_semaphore_destroy(&is_full);
    my_semaphore_destroy(&is_empty);

    //printf de fin pour vérifier les valeurs
    //printf("Total produced items: %d\n", produced);
    //printf("Total consumed items :%d\n",consumed);
    // printf("All producers have finished. Buffer content:\n");
    // for (int i = 0; i < BUFFER_SIZE; i++) {
    //     printf("buffer[%d] = %d\n", i, buffer[i]);
    // }

    return 0;
}