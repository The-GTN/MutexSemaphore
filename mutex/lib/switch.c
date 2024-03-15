 #include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include "../header/switch.h"
#include "../header/hardware.h"
#include "../header/hwconfig.h"

/////// Les constantes
// nombre magique des contextes
#define CTXMAGIC 0xACDC
// contexte courant
static struct ctx_s *current_ctx = NULL;
// anneau des contextes existants
static struct ctx_s *ring = NULL;
// contexte du programme main
static struct ctx_s ctx_main;


// Les signatures de fonctions
void start_current();
void switch_to_ctx(struct ctx_s *newctx);
int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *arg);
void empty_it();
void timer_it();
void irq_enable();
void irq_disable();
void checkDeadLockError();


////////////////////////////////////////////Contexte////////////////////////////////////////////

// initialise un contexte d'execution
int init_ctx(struct ctx_s *ctx, int stack_size, func_t f, void *arg){
  // pile d'execution du contexte
  ctx->ctx_base = malloc(stack_size);
  // pointeur de début d'execution
  ctx->ctx_esp = ctx->ctx_base + stack_size - sizeof(int);
  // pointeur de fin d'execution
  ctx->ctx_ebp = ctx->ctx_esp;
  // nombre magique du contexte, pour être sûr que c'est nous qui l'avons instancié
  ctx->ctx_magic = CTXMAGIC;
  // fonction utilisé par le contexte
  ctx->ctx_f = f;
  // les arguments utilisés dans la fonction du contexte
  ctx->ctx_arg = arg;
  // état courant du contexte
  ctx->ctx_state = CTX_INIT;
  // prochain contexte dans l'anneau des contextes
  ctx->ctx_next = NULL;
  // si bloqué dans un semaphore, le prochain contexte bloqué par le même sémaphore
  ctx->ctx_sem_next = NULL;
  // si bloqué par un mutex, le prochain contexte bloqué par le même mutex
  ctx->ctx_mutex_next = NULL;
  // mutex possédés par le contexte
  ctx->ctx_mutex_owned = NULL;
  return 1;
}

// création de contexte avec une taille stack_size de pile d'execution, utilisant la fonction f avec les arguments arg
int create_ctx( int stack_size, func_t f, void *arg){

  // on initialise la structure de donnée
  struct ctx_s *new;
  new = malloc(sizeof(struct ctx_s));
  assert(new);
  init_ctx(new, stack_size, f, arg);

  // on insère le contexte dans l'anneau des contextes existants
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


// Ordonnancement des contextes
void yield(){
  // si il y a un contexte courant, on va à son suivant dans l'ordre de l'anneau
  if(current_ctx != NULL){
    switch_to_ctx(current_ctx->ctx_next);
  }
  // sinon si il n'y a que l'anneau, on met le début de l'anneau en tant que current
  else if(ring != NULL){
    switch_to_ctx(ring);
  }
}


// Changement de contextes
void switch_to_ctx(struct ctx_s *newctx){

  // s'il y avait un contexte courant, on enregistre les esp et ebp courant dans ce contexte
  if(current_ctx != NULL){
    asm ("movl %%esp , %0"
        : "=r"(current_ctx->ctx_esp));
    asm ("movl %%ebp , %0"
        : "=r"(current_ctx->ctx_ebp));
  }
  // sinon, on enregistre les esp et ebp de l'execution du main dans le contexte principale
  else {
    asm ("movl %%esp , %0"
        : "=r"(ctx_main.ctx_esp));
    asm ("movl %%ebp , %0"
        : "=r"(ctx_main.ctx_ebp));
  }

  // on vérifie le nombre magique du nouveau contexte
  assert(newctx->ctx_magic == CTXMAGIC);

  // si le nouveau contexte est bloqué, on regarde le suivant (interblocage si tous bloqué)
  while(newctx->ctx_state == CTX_SEM_BLK){
    newctx = newctx->ctx_next;
  }

  // le nouveau contexte devient le contexte courant
  current_ctx = newctx;

  // on affecte les registres esp et ebp aux esp et ebp du nouveau contexte
  asm("movl %0, %%esp"
      :
      : "r"(current_ctx->ctx_esp));
  asm("movl %0, %%ebp"
      :
      : "r"(current_ctx->ctx_ebp));

  // si le contexte est encore en état d'initialisation, on peut lancer la fonction
  if(current_ctx->ctx_state == CTX_INIT) start_current();

  return;
}


// démarre le contexte courant
void start_current() {
  current_ctx->ctx_state = CTX_EXEC; //passe le contexte dans un état d'execution
  current_ctx->ctx_f(current_ctx->ctx_arg); //lance la fonction du contexte avec les arguments
  current_ctx->ctx_state = CTX_END; //passe le contexte au status terminé

  // on réaffecte les registres esp et ebp aux esp et ebp du contexte main intial (pour terminer le programme)
  asm("movl %0, %%esp"
    :
    : "r"(ctx_main.ctx_esp));
  asm("movl %0, %%ebp"
    :
    : "r"(ctx_main.ctx_ebp));
  return;
}

////////////////////////////////////////////Timer////////////////////////////////////////////

// fonction vide pour certains indices de IRQVECTOR
void empty_it(void) { return; }

// fonction qui déclenche le changement de contexte
void timer_it() {
    _out(TIMER_ALARM,0xFFFFFFFE);
    yield();
}

// lance des interuptions sur timer pour changer de contexte tous les x temps
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

//réactive les interruptions par timer
void irq_enable() {
  _mask(1);
}

//désactive les interruptions par timer
void irq_disable() {
  _mask(15);
}

////////////////////////////////////////////Semaphore////////////////////////////////////////////

// initialse un semaphore
void sem_init(struct sem_s *sem, int count){
  sem->sem_cpt = count;
  sem->sem_first_ctx = NULL;
}


// On remet un jeton dans le semaphore
void sem_up(struct sem_s *sem){
  irq_disable(); //désactive les interruptions par timer
  sem->sem_cpt++; //remet un jeton dans le semaphore
  if(sem->sem_cpt <= 0){
    sem->sem_first_ctx->ctx_state = CTX_EXEC; //affecte au contexte le statut d'execution
    sem->sem_first_ctx = sem->sem_first_ctx->ctx_sem_next; //enleve un contexte de la liste d'attente
    irq_enable(); //réactive les interruptions par timer
  }else{
    irq_enable(); //réactive les interruptions par timer
  }
}

// Prend un jeton dans le semaphore
void sem_down(struct sem_s *sem){
  irq_disable();//désactive les interruptions par timer
  sem->sem_cpt--;//decremente le nombre de jeton disponible
  if(sem->sem_cpt < 0){
    current_ctx->ctx_state = CTX_SEM_BLK;//affecte au contexte le statut bloqué par semaphore

    //ajoute le contexte courant a la liste des bloqués par le semaphore
    current_ctx->ctx_sem_next = sem->sem_first_ctx;
    sem->sem_first_ctx = current_ctx;
    irq_enable();//réactive les interruptions par timer
    yield();
  }else{
    irq_enable();//réactive les interruptions par timer
  }
}

////////////////////////////////////////////Mutex////////////////////////////////////////////

// initialise un mutex
void mutex_init(struct mutex_s *mutex){
  mutex->mutex_owner_ctx = NULL;
  mutex->mutex_locked = 0;
  mutex->mutex_first_ctx = NULL;
  mutex->mutex_next = NULL;
}

//tente le verrouillage du mutex
void mutex_lock(struct mutex_s *mutex){

  irq_disable();//désactive les interruptions par timer

  //ajout du contexte courant a la fin de la liste des contextes bloqués par le mutex
  if(mutex->mutex_first_ctx == NULL){
    mutex->mutex_first_ctx = current_ctx;
  }else{
    struct ctx_s * next_ctx;
    next_ctx = mutex->mutex_first_ctx;
    while(next_ctx->ctx_mutex_next != NULL){
      next_ctx = next_ctx->ctx_mutex_next;
    }
    next_ctx->ctx_mutex_next = current_ctx;
  }

  // état du contexte courant passe à bloqué par un mutex
  current_ctx->ctx_state = CTX_MUTEX_BLK;

  //détection d'interblocages par mutex
  checkDeadLockError();

  irq_enable();//réactive les interruptions par timer

  //attend que le mutex se libère et que le contexte courant soit le premier dans la liste des bloqués par le mutex
  while (mutex->mutex_locked==1 || mutex->mutex_first_ctx!=current_ctx);

  irq_disable();//désactive les interruptions par timer

  //met a jour les informations du mutex
  mutex->mutex_owner_ctx = current_ctx;
  mutex->mutex_locked = 1;
  //met à jour l'etat du contexte
  current_ctx->ctx_state = CTX_EXEC;

  //enleve le contexte courant de la liste des contextes bloqués par le mutex
  mutex->mutex_first_ctx = mutex->mutex_first_ctx->ctx_mutex_next;
  current_ctx->ctx_mutex_next = NULL;

  //ajout a la liste des mutexs possédé par le contexte courant
  mutex->mutex_next = current_ctx->ctx_mutex_owned;
  current_ctx->ctx_mutex_owned = mutex;

  irq_enable();//réactive les interruptions par timer
}

//tente de deverrouiller le mutex
void mutex_unlock(struct mutex_s *mutex){
  irq_disable();//désactive les interruptions par timer

  //déverrouillage seulement possible si le contexte courant est le propriétaire du mutex
  if(mutex->mutex_owner_ctx==current_ctx){
    mutex->mutex_owner_ctx = NULL; // le mutex n'a plus de propriétaire
    mutex->mutex_locked = 0;  //déverrouille le mutex

    //retrait du mutex de la liste des mutex possédés par le contexte courant
    if(current_ctx->ctx_mutex_owned == mutex){
      current_ctx->ctx_mutex_owned = current_ctx->ctx_mutex_owned->mutex_next;
      mutex->mutex_next = NULL;
    }else{
      struct mutex_s * next_mutex;
      next_mutex = current_ctx->ctx_mutex_owned;
      while(next_mutex->mutex_next != mutex){
        next_mutex = next_mutex->mutex_next;
      }
      next_mutex->mutex_next = next_mutex->mutex_next->mutex_next;
      mutex->mutex_next = NULL;
    }
  }

  irq_enable();//réactive les interruptions par timer
}

//Verifie si un interbloquage va survenir
void checkDeadLockError() {
  int nb_not_blocked = 0;
  struct ctx_s * next_ctx_ring = ring->ctx_next;
  struct ctx_s * start_ring = ring;

  //compte le nombre de contextes non bloqué
  if(ring->ctx_state != CTX_MUTEX_BLK){
    nb_not_blocked += 1;
  }
  while(start_ring != next_ctx_ring && nb_not_blocked == 0) {
    if(next_ctx_ring->ctx_state != CTX_MUTEX_BLK){
      nb_not_blocked += 1;
    }
    next_ctx_ring = next_ctx_ring->ctx_next;
  }
  //si aucun contexte n'est pas bloqué alors il y a interbloquage
  if(nb_not_blocked == 0) {
    fprintf(stderr, "DeadLock Error\n");
  	exit(EXIT_FAILURE);
  }
}
