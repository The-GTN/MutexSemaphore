#ifndef _SWITCH_H_
#define _SWITCH_H_

typedef void (func_t) (void*);

//different états possibles pour un contexte
enum ctx_state_e{CTX_INIT, CTX_EXEC, CTX_END, CTX_SEM_BLK, CTX_MUTEX_BLK};

//structure d'un contexte
struct ctx_s{
  unsigned char* ctx_base;
  void * ctx_esp; // pointeur de début d'execution
  void * ctx_ebp; // pointeur de fin d'execution
  unsigned ctx_magic; // nombre magique du contexte, pour être sûr que c'est nous qui l'avons instancié
  func_t * ctx_f; // fonction utilisé par le contexte
  void * ctx_arg; // les arguments utilisés dans la fonction du contexte
  enum ctx_state_e ctx_state; // état courant du contexte
  struct ctx_s * ctx_next; // prochain contexte dans l'anneau des contextes
  struct ctx_s * ctx_sem_next; // si bloqué dans un semaphore, le prochain contexte bloqué par le même sémaphore
  struct ctx_s * ctx_mutex_next; // si bloqué par un mutex, le prochain contexte bloqué par le même mutex
  struct mutex_s * ctx_mutex_owned; // mutex possédés par le contexte
};

//structure d'un semaphore
struct sem_s{
  int sem_cpt; //capacité du semaphore
  struct ctx_s * sem_first_ctx; //pointeur vers le premier contexte en attente du semaphore (liste chainé)
};

//structure d'un mutex
struct mutex_s{
  struct ctx_s * mutex_owner_ctx; //pointeur vers le contexte propriétaire du mutex
  int mutex_locked; //indique si le mutex est bloqué ou non
  struct ctx_s * mutex_first_ctx; //pointeur vers le premier contexte en attente du mutex (liste chainé)
  struct mutex_s * mutex_next; //pointeur constituant un mayon de la liste chainée des mutex possedé par un contexte
};



///////////fonctions exportées de la bibliothèque
extern int create_ctx( int stack_size, func_t f, void *arg);

//extern void switch_to_ctx(struct ctx_s *newctx);
extern void yield();
extern void start_schedule();

extern void sem_up(struct sem_s *sem);
extern void sem_down(struct sem_s *sem);
extern void sem_init(struct sem_s *sem, int count);

extern void mutex_init(struct mutex_s *mutex);
extern void mutex_lock(struct mutex_s *mutex);
extern void mutex_unlock(struct mutex_s *mutex);

#endif
