#include "../header/switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define STACKSIZE 16384

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void f_ping(void *arg);
void f_pong(void *arg);

int main(int argc, char * argv[]){
  create_ctx(STACKSIZE, f_ping, NULL);
  create_ctx(STACKSIZE, f_pong, NULL);

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
