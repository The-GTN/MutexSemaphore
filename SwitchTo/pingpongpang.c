#include "switch.h"
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#define STACKSIZE 16384

struct ctx_s ctx_ping;
struct ctx_s ctx_pong;
struct ctx_s ctx_pang;

void f_ping(void *arg);
void f_pong(void *arg);
void f_pang(void *arg);

int main(int argc, char * argv[]){
  init_ctx(&ctx_ping, STACKSIZE, f_ping, NULL);
  init_ctx(&ctx_pong, STACKSIZE, f_pong, NULL);
  init_ctx(&ctx_pang, STACKSIZE, f_pang, NULL);

  switch_to_ctx(&ctx_ping);

  exit(EXIT_SUCCESS);
}

void f_ping(void *args){
    printf("A");
    switch_to_ctx(&ctx_pong);
    printf("B");
    switch_to_ctx(&ctx_pong);
    printf("C");
    switch_to_ctx(&ctx_pong);
}

void f_pong(void *args){
    printf("1");
    switch_to_ctx(&ctx_pang);
    printf("2");
    switch_to_ctx(&ctx_pang);
}

void f_pang(void *args){
    printf("-");
    switch_to_ctx(&ctx_ping);
    printf("_");
    switch_to_ctx(&ctx_ping);
}
