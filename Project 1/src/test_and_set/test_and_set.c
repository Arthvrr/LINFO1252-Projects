#include "test_and_set/test_and_set.h"


//2.1 Implémentation de l'algorithme Test and Set
//xchg = effectue un échange atomique entre une valeur en mémoire et une valeur dans un registre.


void init_lock(verrou *current_lock){
    current_lock->is_locked = 0; //définir le verrou comme libre au départ
}


void lock(verrou *current_lock){

    int new = 1; //on essaie de verrouiller le verrou
    int old;

    //partie assembleur in-line
    do {
        __asm__(
            "xchg %0, %1"
            : "=r" (old), "=m" (current_lock->is_locked)  //sorties
            : "0" (new), "m" (current_lock->is_locked)    //entrées
            : "memory"
        );
    } while (old != 0);
}


void unlock(verrou *current_lock){

    //partie assembleur in-line
    __asm__(
        "movl $0, %0"
        : "=m"(current_lock->is_locked)
        :                              
        : "memory"
    );

}


verrou mon_verrou;


void* thread_func(void* arg) {

    pthread_t thread_id = pthread_self();

    //on essaye d'acquérir le verrou
    //printf("Thread %lu: tentative de verrouillage\n", thread_id);
    lock(&mon_verrou);

    //une fois le verrou acquis, on exécute la section critique
    //printf("Thread %lu: verrouillé\n", thread_id);
    sleep(1);  //simulation de travail dans la section critique
    //printf("Thread %lu: fin de la section critique\n", thread_id);

    //libérer le verrou
    unlock(&mon_verrou);
    //printf("Thread %lu: déverrouillé\n", thread_id);

    return NULL;
}

