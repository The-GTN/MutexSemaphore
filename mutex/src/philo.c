#include "../header/switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

// Nombre d'étape pour attendre, nombre intentionnellement excessif
#define STEP (1<<25)
// Taille maximal pour la pile d'execution du contexte
#define STACKSIZE 16384
// Nombre de fourchette (mutex)
#define NBF 5
// Nombre de philosophe (contexte)
#define NBPHILO 5
// Constante LIFE à TRUE (donc à 1)
#define LIFE 1


// notre liste de fourchette, chaque fourchette est représentée par un mutex
static struct mutex_s fs[NBF];


// signatures des fonctions
void philo (void *args);
void faim(int num);
void prendre_fourchette_gauche(int num);
void prendre_fourchette_droite(int num);
void manger(int num);
void deposer_fourchettes(int num);
void penser();
void easterEgg();

// fonction principale de main, initialise les contextes des philosophe, les mutex des fourchettes et démmare le schedule
int main(int argc, char * argv[]){

  // on initialise les contextes correspondant aux philosophes
  for (int i = 0; i< NBPHILO;i++){
    create_ctx(STACKSIZE, philo, i);
  }

  // on initialise les mutex correspondant aux fourchettes
  for (int i = 0; i< NBF;i++){
    mutex_init(&fs[i]);
  }

  // à décommenter si on souhaite ajouter un contexte indépendant, qui évitera l'interblocage
  //create_ctx(STACKSIZE, easterEgg, NULL);

  // on déclenche l'execution des contextes
  start_schedule();
  printf("\n");

  // fin du code
  exit(EXIT_SUCCESS);
}

// cycle de vie du philosophe.
void philo (void *args){
    int num = (int) args;
    while(LIFE) {
      penser();
      faim(num);
      prendre_fourchette_gauche(num);
      penser();
      prendre_fourchette_droite(num);
      manger(num);
      deposer_fourchettes(num);
    }
}

//print que le philosophe num a faim
void faim(int num){
    printf("\n\nJe suis le philosophe %d, et j'ai faim !",num);
}

//prend la fourchette gauche du philosophe num. Elle print la tentative, lock le mutex et print la réussite.
void prendre_fourchette_gauche(int num){
    printf("\nJe suis le philosophe %d, et j'essaie de prendre la fourchette %d qui est à ma gauche",num,num);
    mutex_lock(&fs[num]);
    printf("\nJe suis le philosophe %d, j'ai pris la fourchette %d qui est à ma gauche",num,num);
}

//prend la fourchette droite du philosophe num. Elle print la tentative, lock le mutex et print la réussite.
void prendre_fourchette_droite(int num){
    int numD = (num+1)%NBF;
    printf("\nJe suis le philosophe %d, et j'essaie de prendre la fourchette %d qui est à ma droite",num,numD);
    mutex_lock(&fs[numD]);
    printf("\nJe suis le philosophe %d, et j'ai pris la fourchette %d qui est à ma droite",num,numD);
}

//print que je philosophe num est entrain de manger
void manger(int num) {
    printf("\nJe suis le philosophe %d, et je mange !",num);
}

//depose les fourchettes tu philosophe num. Elle print l'action et libere les mutexs
void deposer_fourchettes(int num) {
    int numD = (num+1)%NBF;
    printf("\nJe suis le philosophe %d, et, pour penser, je depose mes fourchettes %d et %d\n\n",num,num,numD);
    mutex_unlock(&fs[num]);
    mutex_unlock(&fs[numD]);
}

//penser fait "attendre" avant de poursuivre les opérations
void penser() {
  for(int i = 0; i< STEP;i++);
}

//print avec certains délais une phrase
void easterEgg(void* args) {
  while(LIFE) {
    penser();
    printf("\n Pendant ce temps là, deux experts, Suliac et Antoine, sont en train d'écrire la documention...\n");
    penser();
  }
}
