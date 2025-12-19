// https://sites.uclouvain.be/SystInfo/notes/Theorie/html/C/malloc_implementation.html
// https://github.com/ComprosoftCEO/MyMalloc/blob/master/malloc.c <-- implem de malloc en double linked list (trouvé sur internet)
// https://moss.cs.iit.edu/cs351/slides/slides-malloc.pdf

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#define HEAP_SIZE 64000

uint8_t MY_HEAP[HEAP_SIZE]; //tableau de 64000 octets de base



typedef struct Block{
    size_t size;        //taille en octet
    int is_free;        //booléen si block est libre (1) ou non (0)
    struct Block *prev; //pointeur vers le block précédent
    struct Block *next; //pointeur vers le prochain block
} Block;


Block *new_list = (Block *)MY_HEAP; //convertit MY_HEAP en pointeur de type Block*



void init(){
    new_list->size = HEAP_SIZE - sizeof(Block); //taille initiale de l'espace libre
    new_list->is_free = 1; //true
    new_list->prev = NULL;
    new_list->next = NULL;
    printf("Initialization function called.\n");
}


void split_block(Block *current, size_t size){   
    /*création nouveau block après partie de mémoire actuelle
      update des size
      update des prev et next
      utilisé avec my_malloc()
    */
    
    //si taille du block courant est trop petite pour être scindée en 2
    if (current->size <= size + sizeof(Block)) return;


    Block *newBlock = (Block *)((uint8_t *)current + sizeof(Block) + size);

    //vérifie que l'adresse après le block newBlock ne dépasse pas la limite de la heap
    if ((uint8_t *)newBlock + sizeof(Block) > MY_HEAP + HEAP_SIZE) return;

    newBlock->size = current->size - sizeof(Block) - size; //size restante = size actuelle - (bloc metadonnées + size)
    newBlock->is_free = 1;
    newBlock->next = current->next;
    newBlock->prev = current;
    
    if(current->next != NULL){
        current->next->prev = newBlock;
    }
    current->next = newBlock;
    current->size = size;

}


void merge_block(Block *block){
    /*
    permet de fusionner plusieurs block libres entre eux
    utilisé avec my_free()
    */

    //MERGE APRÈS
    if (block->next && block->next->is_free) {

        //assure que le bloc suivant est bien dans les limites
        if ((uint8_t *)block->next < MY_HEAP || (uint8_t *)block->next >= MY_HEAP + HEAP_SIZE) return;

        block->size += sizeof(Block) + block->next->size;
        block->next = block->next->next;
        
        //si un bloc suivant existe encore
        if (block->next) {
            block->next->prev = block; //met à jour le précédent du bloc suivant
        }
    }


    //MERGE AVANT
    if (block->prev && block->prev->is_free) {

        //assure que le bloc précédent est bien dans les limites
        if ((uint8_t *)block->prev < MY_HEAP || (uint8_t *)block->prev >= MY_HEAP + HEAP_SIZE) return;

        block->prev->size += sizeof(Block) + block->size;
        block->prev->next = block->next;

        //si un bloc suivant existe encore
        if (block->next) {
            block->next->prev = block->prev; //met à jour le précédent du bloc suivant
        }
    }
}


void my_free(void *pointer){

    if (pointer == NULL) { //si le pointeur est NULL, ne rien faire
        return;
    }

    //vérifie que le bloc est bien dans MY_HEAP pour éviter les erreurs de segmentation
    if ((uint8_t *)pointer < MY_HEAP || (uint8_t *)pointer >= MY_HEAP + HEAP_SIZE) {
        printf("Attempt to free memory outside MY_HEAP limits\n");
        return;
    }

    //parcourt la liste pour vérifier que le pointeur appartient à un bloc alloué
    Block *current = new_list;
    int block_found = 0; //falsee

    while (current != NULL) {
        void *start_of_data = (void *)((uint8_t *)current + sizeof(Block));
        
        //vérifie si le pointeur correspond au début de la zone de données
        if (start_of_data == pointer) {
            block_found = 1;

            //vérifie si le bloc est déjà libre
            if (current->is_free) {
                printf("Attempting to release a block that has already been released\n");
                return;
            } else {
                current->is_free = 1; //marque le bloc comme libre
                merge_block(current);  //fusionne avec les blocs libres adjacents
            }
            break;
        }
        current = current->next;
    }

    //si on ne trouve pas le block à libérer, retourner une erreur
    if (!block_found) {
        printf("Attempt to free a block that has never been allocated\n");
        return;
    }

}


/*
vérifier avant d'allouer :
1. si on est à la fin
2. si le bloc itéré est occupé ou non
3. si on a assez d'espace pour stocker size

si zone trop grande demandée par size, on scinde en une zone réservée et une zone libre
*/
void *my_malloc(size_t size){

    int int_size = (int) size; //on convertit size en int pour gérer le cas où on reçoit une size négative en paramètre

    //si taille demandée négative ou nulle ou dépasse le nombre d'octets
    if (int_size <= 0 || size + sizeof(Block) > HEAP_SIZE){
        printf("Asked size is unallowed\n");
        return NULL;
    }
    Block *current = new_list;
    while (current){

        //on check si la taille restante est suffisante et si le bloc est libre
        if((current->size >= size + sizeof(Block)) && current->is_free){

            current->is_free = 0; //on indique que le bloc n'est pas libre
            split_block(current,size);
            current->size = size; //on met à jour la taille du block courant
            printf("Allocated %zu bytes\n", size);
            return (void*)((uint8_t *)current + sizeof(Block));
        }
        current = current -> next;
    }
    printf("Failed to allocate memory for %zu bytes\n", size);
    return NULL;

}


//TESTS

int malloc_test(){
    //TESTS SUR MY_MALLOC

    printf("\n--- TESTS SUR MY_MALLOC ---\n\n");
    // Test 1: Allocation d'une taille trop grande
    printf("Test 1: Allocation d'une taille trop grande (70000 octets).\n");
    void *ptr1 = my_malloc(70000);
    if (ptr1 == NULL) {
        printf("Test 1: Allocation échouée comme prévu.\n");
    } else {
        printf("Test 1: Allocation réussie alors qu'elle aurait dû échouer.\n");
        my_free(ptr1); // Si cela passe, il faut le libérer
    }

    // Test 2: Allocation d'une taille négative
    printf("Test 2: Allocation d'une taille négative (-10 octets).\n");
    void *ptr2 = my_malloc(-10);
    if (ptr2 == NULL) {
        printf("Test 2: Allocation échouée comme prévu.\n");
    }

    // Test 3: Libération d'un pointeur non alloué
    printf("Test 3: Libération d'un pointeur non alloué.\n");
    my_free(NULL);  // Cela ne devrait rien faire

    // Test 4: Libération d'un pointeur déjà libéré
    printf("Test 4: Libération d'un pointeur déjà libéré.\n");
    void *ptr3 = my_malloc(50);
    if (ptr3 != NULL) {
        my_free(ptr3);
        my_free(ptr3);  // Cela devrait être un cas problématique
        printf("Test 4: Libération d'un pointeur déjà libéré.\n");
    }

    // Test 5: Libération d'une mémoire en dehors de MY_HEAP
    printf("Test 5: Libération d'une mémoire en dehors de MY_HEAP.\n");
    my_free((void *)((uint8_t *)MY_HEAP + HEAP_SIZE + 1));  // Adresse en dehors de MY_HEAP

    // Test 6: Allocation d'une taille valide et vérification de la libération
    printf("Test 6: Allocation d'une taille valide (100 octets).\n");
    void *ptr4 = my_malloc(100);
    if (ptr4 != NULL) {
        printf("Test 6: Allocation réussie.\n");
        my_free(ptr4);
        printf("Test 6: Libération réussie.\n");
    }

    return 0;
}

int free_test(){
    //TEST SUR MY_FREE

    
    printf("\n--- TESTS SUR MY_FREE ---\n\n");
    // Test 1: Libération d'un pointeur nul
    printf("Test 1: Libération d'un pointeur nul.\n");
    my_free(NULL);  // Cela ne devrait rien faire

    // Test 2: Libération d'un bloc alloué
    printf("Test 2: Libération d'un bloc alloué.\n");
    void *ptr5 = my_malloc(20);
    if (ptr5 != NULL) {
        my_free(ptr5);  // Libération du bloc alloué
        printf("Test 2: Libération réussie d'un bloc alloué.\n");
    }

    // Test 3: Libération d'un pointeur qui n'a jamais été alloué
    printf("Test 3: Libération d'un pointeur qui n'a jamais été alloué.\n");
    void *ptr6 = (void *)((uint8_t *)MY_HEAP + 100); // Adresse aléatoire dans MY_HEAP
    my_free(ptr6);  // Cela devrait afficher une erreur

    // Test 4: Libération d'un bloc déjà libéré
    printf("Test 4: Libération d'un bloc déjà libéré.\n");
    void *ptr7 = my_malloc(30);
    if (ptr7 != NULL) {
        my_free(ptr7);  // Libération initiale
        my_free(ptr7);  // Répétée - cela devrait être géré
        printf("Test 4: Libération réussie d'un bloc déjà libéré (comportement à surveiller).\n");
    }

    // Test 5: Libération d'un bloc avec des voisins (fusion)
    printf("Test 5: Libération d'un bloc avec des voisins (fusion).\n");
    void *ptr8 = my_malloc(200);
    void *ptr9 = my_malloc(300);
    if (ptr8 != NULL && ptr9 != NULL) {
        my_free(ptr8);  // Libérer le premier bloc
        my_free(ptr9);  // Libérer le deuxième bloc - cela devrait fusionner
        printf("Test 5: Libération réussie avec fusion.\n");
    }

    // Test 6: Libération d'un bloc au milieu d'une chaîne de blocs
    printf("Test 6: Libération d'un bloc au milieu d'une chaîne de blocs.\n");
    void *ptr10 = my_malloc(50);
    void *ptr11 = my_malloc(50);
    void *ptr12 = my_malloc(50);
    if (ptr10 != NULL && ptr11 != NULL && ptr12 != NULL) {
        my_free(ptr11);  // Libérer le bloc du milieu
        // Ici, vérifiez l'intégrité de la liste après la libération
        printf("Test 6: Libération réussie d'un bloc au milieu d'une chaîne.\n");
    }

    // Test 7: Libération d'un bloc fusionné
    printf("Test 7: Libération d'un bloc fusionné.\n");
    void *ptr13 = my_malloc(100);
    void *ptr14 = my_malloc(200);
    if (ptr13 != NULL && ptr14 != NULL) {
        my_free(ptr13);  // Libération du premier bloc
        my_free(ptr14);  // Libération du deuxième bloc
        printf("Test 7: Libération réussie de blocs fusionnés.\n");
    }

    // Test 8: Libération d'un bloc hors des limites de MY_HEAP
    printf("Test 8: Libération d'un bloc hors des limites de MY_HEAP.\n");
    my_free((void *)((uint8_t *)MY_HEAP + HEAP_SIZE + 1));  // Adresse hors limites

    return 0;
}


int main(int argc, char *argv[]){
    printf("Taille de Block en octets : %zu\n\n", sizeof(Block));
    init();
    malloc_test();
    free_test();
    
    return 0;
}
