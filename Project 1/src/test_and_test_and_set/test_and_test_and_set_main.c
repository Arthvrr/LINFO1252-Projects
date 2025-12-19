#include "test_and_test_and_set/test_and_test_and_set_main.h"

/*
Fonction qui nous permet d'avoir un main() pour test_and_test_and_set 
--> sinon redondance avec les main de test_and_test_and_set_philosoph, test_and_test_and_set_prod_cons, test_and_test_and_set_reader_writer
Fonction utilisée au 2.2 dans test_and_test_and_set_perf.sh
*/

#define TOTAL_CRITICAL_SECTIONS 32768

verrou mon_verrou;
int shared_counter = 0; //variable partagée pour la section critique


void* test_func(void* arg) {
    pthread_t thread_id = pthread_self();
    int section_count = *((int*)arg); //récupérer le nombre de sections critiques à effectuer

    //printf("Thread %lu: tente de verrouiller\n", thread_id);
    for (int i = 0; i < section_count; i++) { //on effectue 32768/N sections critique
        lock(&mon_verrou); //verrouillage

        //section critique
        //printf("Thread %lu: verrou acquis, entre dans la section critique (%d/%d)\n", thread_id, i + 1, section_count);
        int local_copy = shared_counter;
        shared_counter = local_copy + 1;

        unlock(&mon_verrou); //déverrouillage
        //printf("Thread %lu: verrou libéré\n", thread_id);
    }

    return NULL;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        //printf("Please give us the number of threads you want !\n");
        return 1;
    }

    int n_threads = atoi(argv[1]); // on convertit de *char en int avec atoi()

    if (n_threads <= 0) {
        //printf("Number of threads should be positive !\n");
        return 1;
    }

    //calcul du nombre de sections critiques par thread
    int section_per_thread = TOTAL_CRITICAL_SECTIONS / n_threads;
    //printf("Nombre de sections critiques par thread: %d\n", section_per_thread);

    pthread_t threads[n_threads];
    int i;

    init_lock(&mon_verrou);

    //création des threads
    for (i = 0; i < n_threads; i++) {
        // Passer le nombre de sections critiques à chaque thread
        if (pthread_create(&threads[i], NULL, test_func, &section_per_thread) != 0) {
            perror("pthread_create");
            exit(EXIT_FAILURE);
        }
    }

    //attente de la fin des threads
    for (i = 0; i < n_threads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(EXIT_FAILURE);
        }
    }

    //printf("Valeur finale du compteur partagé = %d\n", shared_counter);

    return 0;
}