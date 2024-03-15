#ifndef _TRY_H_
#define _TRY_H_

typedef int (func_t)(int);

struct ctx_s{
  void * ctx_esp;
  void * ctx_ebp;
  unsigned ctx_magic;
};

extern int try(struct ctx_s *pctx, func_t *f, int arg);
extern int throw(struct ctx_s *pctx, int r);

#endif
