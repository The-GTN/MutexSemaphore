#include "../header/switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define STACKSIZE 16384
#define STEP 10

void f_ping(void *arg);
void f_pong(void *arg);

int main(int argc, char * argv[]){
  create_ctx(STACKSIZE, f_ping, NULL);
  create_ctx(STACKSIZE, f_pong, NULL);

  yield();

  exit(EXIT_SUCCESS);
}

void f_ping(void *args){
  printf("A");
  yield();
  printf("B");
  yield();
  printf("C");
  yield();
}

void f_pong(void *args){
  printf("1");
  yield();
  printf("2");
  yield();
}
