#include "../header/switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define N 100
#define STEP (1<<20)
#define STACKSIZE 16384
#define NBPROD 100
#define NBCONS 100

struct sem_s mutex, vide, plein;

void producteur (void *args);
void consommateur (void *args);
void produire_objet();
void mettre_objet();
void retirer_objet();
void utiliser_objet();

int main(int argc, char * argv[]){
  for(int i =0; i<NBPROD;i++) create_ctx(STACKSIZE, producteur, NULL);
  for(int i =0; i<NBCONS;i++) create_ctx(STACKSIZE, consommateur, NULL);

  sem_init(&mutex, 1);
  sem_init(&vide, N);
  sem_init(&plein, 0);

  start_schedule();
  printf("\n");

  exit(EXIT_SUCCESS);
}

void producteur (void *args){
  while(1) {
    for(int i = 0; i< STEP;i++)
      ;
    produire_objet();
    sem_down(&vide);
    sem_down(&mutex);
    mettre_objet();
    sem_up(&mutex);
    sem_up(&plein);
  }
}

void consommateur (void *args){
  while(1) {
    for(int i = 0; i< STEP;i++)
      ;
    sem_down(&plein);
    sem_down(&mutex);
    retirer_objet();
    sem_up(&mutex);
    sem_up(&vide);
    utiliser_objet();
  }
}

void produire_objet(){ printf("\nJe produis un objet"); }
void mettre_objet(){ printf("\nJe mets un objet\n"); }
void retirer_objet(){ printf("\nJe retire un objet"); }
void utiliser_objet(){ printf("\nJ'utilise un objet\n"); }
