#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include "switch.h"
#include <stdio.h>

#define CTXMAGIC 0xACDC
static struct ctx_s *current_ctx = NULL;
void start_current();

static struct ctx_s ctx_main;

int init_ctx(struct ctx_s *ctx, int stack_size,func_t f,void *arg){
  ctx->ctx_base = malloc(stack_size);
  ctx->ctx_esp = ctx->ctx_base + stack_size - sizeof(int);
  ctx->ctx_ebp = ctx->ctx_esp;
  ctx->ctx_magic = CTXMAGIC;
  ctx->ctx_f = f;
  ctx->ctx_arg = arg;
  ctx->ctx_state = CTX_INIT;

  return 1;
}

void switch_to_ctx(struct ctx_s *newctx){
  if(current_ctx != NULL){
    asm ("movl %%esp , %0"
        : "=r"(current_ctx->ctx_esp));
    asm ("movl %%ebp , %0"
        : "=r"(current_ctx->ctx_ebp));
  }
  else {
    asm ("movl %%esp , %0"
        : "=r"(ctx_main.ctx_esp));
    asm ("movl %%ebp , %0"
        : "=r"(ctx_main.ctx_ebp));
  }

  assert(newctx->ctx_magic == CTXMAGIC);
  assert(newctx->ctx_state != CTX_END);

  current_ctx = newctx;

  asm("movl %0, %%esp"
      :
      : "r"(current_ctx->ctx_esp));
  asm("movl %0, %%ebp"
      :
      : "r"(current_ctx->ctx_ebp));

  if(current_ctx->ctx_state == CTX_INIT) start_current();

  return;
}

void start_current() {
  current_ctx->ctx_state = CTX_EXEC;
  current_ctx->ctx_f(current_ctx->ctx_arg);
  current_ctx->ctx_state = CTX_END;

  asm("movl %0, %%esp"
    :
    : "r"(ctx_main.ctx_esp));
  asm("movl %0, %%ebp"
    :
    : "r"(ctx_main.ctx_ebp));
    
  return;
}
