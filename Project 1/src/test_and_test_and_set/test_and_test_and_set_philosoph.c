#include "test_and_test_and_set/test_and_test_and_set_philosoph.h"


//2.5 Adaptation du 1.1 avec nos primitives d'attentes actives --> problème des philosophes


int N; //nombre de philosophes (égal au nombre de fourchettes)
my_semaphore *forks; //tableau de fourchettes contenant les sémaphores


void* philosopher(void *arg) {
    /*
    Fonction exécutée par chaque philosophe
    */

    int id_philosopher = *(int*)arg; //cast vers un entier

    for (int i = 0; i <= 1000000; i++) {

        //printf("Iteration number %d\n", i);

        int left_fork = id_philosopher;
        int right_fork = (left_fork + 1) % N;

        //printf("Philosophe %d pense...\n", id_philosopher);
        
        //ordre asymétrique pour éviter le blocage
        if (id_philosopher % 2 == 0) {

            //philosophe pair : fourchette gauche, puis droite
            //printf("Philosophe %d essaie de prendre la fourchette gauche %d\n", id_philosopher, left_fork);
            my_semaphore_wait(&forks[left_fork]);
            //printf("Philosophe %d a pris la fourchette gauche %d\n", id_philosopher, left_fork);

            //printf("Philosophe %d essaie de prendre la fourchette droite %d\n", id_philosopher, right_fork);
            my_semaphore_wait(&forks[right_fork]);
            //printf("Philosophe %d a pris la fourchette droite %d\n", id_philosopher, right_fork);

        } else {

            //philosophe impair : fourchette droite, puis gauche
            //printf("Philosophe %d essaie de prendre la fourchette droite %d\n", id_philosopher, right_fork);
            my_semaphore_wait(&forks[right_fork]);
            //printf("Philosophe %d a pris la fourchette droite %d\n", id_philosopher, right_fork);

            //printf("Philosophe %d essaie de prendre la fourchette gauche %d\n", id_philosopher, left_fork);
            my_semaphore_wait(&forks[left_fork]);
            //printf("Philosophe %d a pris la fourchette gauche %d\n", id_philosopher, left_fork);
        }

        //printf("Philosophe %d mange...\n", id_philosopher); // Manger, action immédiate

        //libère les fourchettes après avoir mangé
        my_semaphore_signal(&forks[left_fork]);
        my_semaphore_signal(&forks[right_fork]);

        //printf("Philosophe %d a terminé de manger et pense...\n", id_philosopher); // Pense, action immédiate
    }
    return NULL;
}


/*
argc = nombre d'arguments
argv = liste des arguments
*/
int main(int argc, char *argv[]) {

    //printf("Début d'exécution\n");

    //clock_t start = clock();

    if (argc != 2) {
        //printf("Please give us the number of Philosophers !\n");
        return 1;
    }

    N = atoi(argv[1]); //on convertit de *char en int avec atoi()

    if (N <= 0) {
        //printf("Number of Philosophers should be positive !\n");
        return 1;
    }

    //printf("Nombre de philosophes : %d\n", N);

    forks = malloc(N * sizeof(my_semaphore)); //allocation avec malloc()
    for (int i = 0; i < N; i++) {
        my_semaphore_init(&forks[i], 1);  //chaque fourchette est initialisée à 1 (libre)
    }

    pthread_t philosophes[N];
    int ids[N];
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        //printf("Création du thread pour le philosophe %d...\n", i);
        pthread_create(&philosophes[i], NULL, philosopher, &ids[i]); //crée un thread pour chaque philosophe
    }

    //attendre la fin de tous les threads
    for (int i = 0; i < N; i++) {
        pthread_join(philosophes[i], NULL);
    }

    //détruire les sémaphores
    for (int i = 0; i < N; i++) {
        my_semaphore_destroy(&forks[i]);
    }

    free(forks);

    //printf("Simulation terminée\n");
    //clock_t end = clock();
    //double elapsed_time = ((double) (end - start)) / CLOCKS_PER_SEC;
    //printf("Temps d'exécution : %f secondes\n", elapsed_time);
    
    return 0;
}