
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../header/switch.h"
#include "../header/hardware.h"
#include "../header/hwconfig.h"

#define CTXMAGIC 0xACDC
static struct ctx_s *current_ctx = NULL;
static struct ctx_s *ring = NULL;
static struct ctx_s ctx_main;

void start_current();
void switch_to_ctx(struct ctx_s *newctx);
int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *arg);
void empty_it();
void timer_it();
void irq_enable();
void irq_disable();

int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *arg){
  ctx->ctx_base = malloc(stack_size);
  ctx->ctx_esp = ctx->ctx_base + stack_size - sizeof(int);
  ctx->ctx_ebp = ctx->ctx_esp;
  ctx->ctx_magic = CTXMAGIC;
  ctx->ctx_f = f;
  ctx->ctx_arg = arg;
  ctx->ctx_state = CTX_INIT;
  ctx->ctx_next = NULL;
  ctx->ctx_sem_next = NULL;
  return 1;
}

int create_ctx( int stack_size, func_t f, void *arg){
  struct ctx_s *new;
  new = malloc(sizeof(struct ctx_s));
  assert(new);
  init_ctx(new, stack_size, f, arg);

  if(ring == NULL){
    ring = new;
    new->ctx_next = new;
  }
  else{
    new->ctx_next = ring->ctx_next;
    ring->ctx_next = new;
  }

  return 1;
}

void yield(){
  if(current_ctx != NULL){
    switch_to_ctx(current_ctx->ctx_next);
  }else if(ring != NULL){
    switch_to_ctx(ring);
  }
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
  //assert(newctx->ctx_state != CTX_END);

  while(newctx->ctx_state == CTX_SEM_BLK){
    newctx = newctx->ctx_next;
  }

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

void empty_it(void) { return; }

void timer_it() {
    _out(TIMER_ALARM,0xFFFFFFFE);
    yield();
}

void start_schedule() {

  unsigned int i;

    /* init hardware */
    if (init_hardware(INIFILENAME) == 0) {
	fprintf(stderr, "Error in hardware initialization\n");
	exit(EXIT_FAILURE);
    }

    /* dummy interrupt handlers */
    for (i=0; i<16; i++)
	IRQVECTOR[i] = empty_it;

    /* program timer */
    IRQVECTOR[TIMER_IRQ] = timer_it;
    _out(TIMER_PARAM,128+64+32+8); /* reset + alarm on + 8 tick / alarm */
    _out(TIMER_ALARM,0xFFFFFFFE);  /* alarm at next tick (at 0xFFFFFFFF) */

   /* allows all IT */
    _mask(1);

    /* count for a while... */
    for (i=0; i<(1<<28); i++)
	;

  return;
}

void irq_enable() {
  _mask(1);
}

void irq_disable() {
  _mask(15);
}

void sem_init(struct sem_s *sem, int count){
  sem->sem_cpt = count;
  sem->sem_first_ctx = NULL;
}

void sem_up(struct sem_s *sem){
  irq_disable();
  sem->sem_cpt++;
  if(sem->sem_cpt <= 0){
    sem->sem_first_ctx->ctx_state = CTX_EXEC;
    sem->sem_first_ctx = sem->sem_first_ctx->ctx_sem_next;
    irq_enable();
  }else{
    irq_enable();
  }
}

void sem_down(struct sem_s *sem){
  irq_disable();
  sem->sem_cpt--;
  if(sem->sem_cpt < 0){
    current_ctx->ctx_state = CTX_SEM_BLK;
    current_ctx->ctx_sem_next = sem->sem_first_ctx;
    sem->sem_first_ctx = current_ctx;
    irq_enable();
    yield();
  }else{
    irq_enable();
  }
}
