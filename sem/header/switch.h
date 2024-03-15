#ifndef _SWITCH_H_
#define _SWITCH_H_

typedef void (func_t) (void*);

enum ctx_state_e{CTX_INIT, CTX_EXEC, CTX_END, CTX_SEM_BLK};

struct ctx_s{
  unsigned char* ctx_base;
  void * ctx_esp;
  void * ctx_ebp;
  unsigned ctx_magic;
  func_t * ctx_f;
  void * ctx_arg;
  enum ctx_state_e ctx_state;
  struct ctx_s * ctx_next;
  struct ctx_s * ctx_sem_next;
};

struct sem_s{
  int sem_cpt;
  struct ctx_s * sem_first_ctx;
};

extern int create_ctx( int stack_size, func_t f, void *arg);

//extern void switch_to_ctx(struct ctx_s *newctx);
extern void yield();
extern void start_schedule();
extern void sem_up(struct sem_s *sem);
extern void sem_down(struct sem_s *sem);
extern void sem_init(struct sem_s *sem, int count);

#endif
