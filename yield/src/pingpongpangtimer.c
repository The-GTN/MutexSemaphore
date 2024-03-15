#include "../header/switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define STACKSIZE 16384
#define STEP (1<<20)

void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);

int main(int argc, char * argv[]){
  create_ctx(STACKSIZE, f_ping, NULL);
  create_ctx(STACKSIZE, f_pong, NULL);
  create_ctx(STACKSIZE, f_pang, NULL);

  start_schedule();

  exit(EXIT_SUCCESS);
}

void f_ping(void *args){
  for(int i = 0; i< STEP;i++)
    ;
    printf("A");
    printf("B");
    printf("C");
}

void f_pong(void *args){
  for(int i = 0; i< STEP;i++)
    ;
    printf("1");
    printf("2");
}

void f_pang(void *args){
  for(int i = 0; i< STEP;i++)
    ;
    printf("-");
    printf("_");
}
