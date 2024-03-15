#include "../header/switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define STACKSIZE 16384

void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);

int main(int argc, char * argv[]){
  create_ctx(STACKSIZE, f_ping, NULL);
  create_ctx(STACKSIZE, f_pong, NULL);
  create_ctx(STACKSIZE, f_pang, NULL);

  yield();

  exit(EXIT_SUCCESS);
}

void f_ping(void *args){
  while(1){
    printf("A");
    yield();
    printf("B");
    yield();
    printf("C");
    yield();
  }
}

void f_pong(void *args){
  while(1){
    printf("1");
    yield();
    printf("2");
    yield();
  }
}

void f_pang(void *args){
  while(1){
    printf("-");
    yield();
    printf("_");
    yield();
  }
}
