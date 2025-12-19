#include "prod_cons.h"


//1.2 Problème des producteurs et consommateurs

#define BUFFER_SIZE 8
#define NUMBER_ELEMENTS 131072

int buffer[BUFFER_SIZE]; //tableau d'entiers buffer
int thread_prod; //nombre de threads du producer
int thread_cons; //nombre de threads du consumer
int produced = 0; //nombre d'éléments produits
int consumed = 0; //nombre d'éléments consommés
int idx = 0; //index dans le buffer

pthread_mutex_t mutex;
sem_t is_full;
sem_t is_empty;

void* produce(){
    /*
    Produit des éléments dans le buffer
    Remplit le buffer --> attend donc quand le buffer est plein
    */
    pthread_t thread_id = pthread_self(); //récupérer l'id du thread
    while (true){
        
        for (int i = 0; i < 1000;i++){} //simulation d'un traitement

        sem_wait(&is_empty); //on attend une zone libre dans le buffer pour produire

        //SECTION CRITIQUE
        pthread_mutex_lock(&mutex);

        if(produced >= NUMBER_ELEMENTS){ //si le nombre d'éléments produits est plus grand ou égal au nombre d'éléments, on break
            //printf("Producer thread (ID: %ld) finished. Total produced: %d\n", (long)thread_id, produced); // Ajout de printf ici
            pthread_mutex_unlock(&mutex);
            pthread_exit(NULL); //fin du thread
        }

        buffer[idx] = (int) thread_id; //cast vers int
        produced++;
        idx = (idx + 1) % BUFFER_SIZE;

        sem_post(&is_full); //+1 place remplie dans le buffer
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
        
        sem_wait(&is_full); //on attend une donnée disponible dans le buffer pour la consommer

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

        sem_post(&is_empty); //+1 place vide dans le buffer
        pthread_mutex_unlock(&mutex);

    }
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

    thread_prod = atoi(argv[1]); // on convertit de *char en int avec atoi()
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
    sem_init(&is_full, 0, 0);            
    sem_init(&is_empty, 0, BUFFER_SIZE);

    pthread_t producers[thread_prod];
    pthread_t consumers[thread_cons];

    //créer les threads producer
    for (int i = 0; i < thread_prod; i++) {
        if (pthread_create(&producers[i], NULL, produce, NULL) != 0) {
            perror("Failed to create producer thread");
            return 1;
        }
    }

    //créer les threads consumer
    for (int i = 0; i < thread_cons; i++) {
        if (pthread_create(&consumers[i], NULL, consume, NULL) != 0) {
            perror("Failed to create consumer thread");
            return 1;
        }
    }

    //attendre la fin des threads producer
    for (int i = 0; i < thread_prod; i++) {
        pthread_join(producers[i], NULL);
    }

    //attendre la fin des threads consommateurs
    for (int i = 0; i < thread_cons; i++) {
        pthread_join(consumers[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&is_full);
    sem_destroy(&is_empty);

    //printf de fin pour vérifier les valeurs
    //printf("Total produced items: %d\n", produced);
    //printf("Total consumed items :%d\n",consumed);
    //printf("All producers have finished. Buffer content:\n");
    // for (int i = 0; i < BUFFER_SIZE; i++) {
    //     printf("buffer[%d] = %d\n", i, buffer[i]);
    // }

    return 0;
}