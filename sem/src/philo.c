
#include "../header/switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define STEP (1<<25)
#define STACKSIZE 16384
#define NBF 5
#define NBPHILO 5

static struct sem_s fs[NBF];

void philo (void *args);
void faim(int num);
void prendre_fourchette_gauche(int num);
void prendre_fourchette_droite(int num);
void manger(int num);
void deposer_fourchettes(int num);

int main(int argc, char * argv[]){
  for (int i = 0; i< NBPHILO;i++) create_ctx(STACKSIZE, philo, i);
  for (int i = 0; i< NBF;i++) sem_init(&fs[i], 1);

  start_schedule();
  printf("\n");

  exit(EXIT_SUCCESS);
}

void philo (void *args){
    int num = (int) args;
  while(1) {
    for(int i = 0; i< STEP;i++)
      ;
    faim(num);
    prendre_fourchette_gauche(num);
    for(int i = 0; i< STEP;i++)
      ;
    prendre_fourchette_droite(num);
    manger(num);
    deposer_fourchettes(num);
  }
}

void faim(int num){ printf("\n\nJe suis le philosophe %d, et j'ai faim !",num); }

void prendre_fourchette_gauche(int num){
    printf("\nJe suis le philosophe %d, et j'essaie de prendre la fourchette %d qui est à ma gauche",num,num);
    sem_down(&fs[num]);
    printf("\nJe suis le philosophe %d, j'ai pris la fourchette %d qui est à ma gauche",num,num);
    }

void prendre_fourchette_droite(int num){
    int numD = (num+1)%NBF;
    printf("\nJe suis le philosophe %d, et j'essaie de prendre la fourchette %d qui est à ma droite",num,numD);
    sem_down(&fs[numD]);
    printf("\nJe suis le philosophe %d, et j'ai pris la fourchette %d qui est à ma droite",num,numD);
    }

void manger(int num) {printf("\nJe suis le philosophe %d, et je mange !",num);}

void deposer_fourchettes(int num) {
    int numD = (num+1)%NBF;
    printf("\nJe suis le philosophe %d, et, pour penser, je depose mes fourchettes %d et %d\n\n",num,num,numD);
    sem_up(&fs[num]);
    sem_up(&fs[numD]);
    }
