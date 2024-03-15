#ifndef _SWITCH_H_
#define _SWITCH_H_

typedef void (func_t) (void*);

enum ctx_state_e{CTX_INIT, CTX_EXEC, CTX_END};

struct ctx_s{
  unsigned char* ctx_base;
  void * ctx_esp;
  void * ctx_ebp;
  unsigned ctx_magic;
  func_t * ctx_f;
  void * ctx_arg;
  enum ctx_state_e ctx_state;
};

extern void switch_to_ctx(struct ctx_s *newctx);

extern int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *arg);

#endif
