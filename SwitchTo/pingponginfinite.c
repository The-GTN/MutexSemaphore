#include "switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define STACKSIZE 16384

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;

void f_ping(void *arg);
void f_pong(void *arg);

int main(int argc, char * argv[]){
  init_ctx(&ctx_ping, STACKSIZE, f_ping, NULL);
  init_ctx(&ctx_pong, STACKSIZE, f_pong, NULL);

  switch_to_ctx(&ctx_ping);

  exit(EXIT_SUCCESS);
}

void f_ping(void *args){
  while(1){
    printf("A");
    switch_to_ctx(&ctx_pong);
    printf("B");
    switch_to_ctx(&ctx_pong);
    printf("C");
    switch_to_ctx(&ctx_pong);
  }
}

void f_pong(void *args){
  while(1){
    printf("1");
    switch_to_ctx(&ctx_ping);
    printf("2");
    switch_to_ctx(&ctx_ping);
  }
}
