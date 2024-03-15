#include <assert.h>
#include "try.h"

#define CTXMAGIC 0xACDC
static int throwr;

int try(struct ctx_s *pctx, func_t *f, int arg) {
  asm ("movl %%esp , %0"
      : "=r"(pctx->ctx_esp));
  asm ("movl %%ebp , %0"
      : "=r"(pctx->ctx_ebp));

  pctx->ctx_magic = CTXMAGIC;
  return f(arg);
}

int throw(struct ctx_s *pctx, int r){
  throwr = r;
  assert(pctx->ctx_magic == CTXMAGIC);

  asm("movl %0, %%esp"
      :
      : "r"(pctx->ctx_esp));
  asm("movl %0, %%ebp"
      :
      : "r"(pctx->ctx_ebp));

  return throwr;
}
