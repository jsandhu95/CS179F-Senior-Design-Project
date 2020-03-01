#include "types.h"
#include "user.h"
#define NTHREAD        10  // Maximum number of threads 
#define STACK_SIZE     4096  // Size of stack 


enum states{UNUSED, READY};

struct thread{
  int tid;
  int state;
  uint *sp;
  uint *stack;
};

int nexttid;                            // Next value for tid
int curthread;                          // Current running thread 
struct thread threadtable[NTHREAD];     // Thread table
  

void uthread_init(){
  struct thread *u;

  curthread = 0;
  nexttid = 0;

  for(int i = 0; i < NTHREAD; i++){
    u = &threadtable[i];
    u->tid = 0;
    u->state = UNUSED;
    u->stack = 0; 
    u->sp = 0;
  }
}

void uthread_create(void *func){
  struct thread *u;

  for(u = threadtable; u < &threadtable[NTHREAD]; u++){
    if(u->state == UNUSED){
      u->stack = malloc(STACK_SIZE);
      // Need to build stack
      u->sp = u->stack + (STACK_SIZE/4) - 1;
      u->sp--;
      *u->sp = (uint)func;
      u->sp--;
      *u->sp = (uint)u->sp;
      // Set struct entries
      u->state = READY;
      u->tid = nexttid++;
      return;
    }
  }
  printf(1,"Didn't find UNUSED entry");
  return;
}


void uthread_start(){
  asm("movl %0, %%esp; movl %%esp, %%ebp"
      :
      : "r" (threadtable[0].sp)
  );

  return;
}

void uthread_scheduler(){
  int i = 0;
  int index = 0;
  int curthread_temp = curthread;

  for(i = 1; i <= NTHREAD; i++){
    index = (curthread + i)%NTHREAD;
    if(threadtable[index].state == READY){
      curthread = index;

      // Switch from curthread stack to new stack
      // Store current thread
      asm("pushl %%ebp; movl %%esp, %0"
          : "=r" (threadtable[curthread_temp].sp)
         );
      // Load new thread
      asm("movl %0, %%esp; popl %%ebp"
          :
          : "r" (threadtable[index].sp)
         );
      return;
    }
  }
  exit();
}

void uthread_exit(){
  threadtable[curthread].tid = 0;
  free(threadtable[curthread].sp);
  threadtable[curthread].sp = 0;
  threadtable[curthread].stack = 0;
  threadtable[curthread].state = UNUSED;
  uthread_scheduler();
}


void uthread_yield(){
  threadtable[curthread].state = READY;
  uthread_scheduler();
}


void ping(){
  for(int i = 0; i < 10; i++){
    printf(1,"ping #%d\n", i);
    uthread_yield();
  }
  uthread_exit();
}


void pong(){
  for(int i = 0; i < 5; i++){
    printf(1,"pong #%d\n", i);
    uthread_yield();
  }
  uthread_exit();
}


int main(){
  uthread_init();

  uthread_create(ping);
  uthread_create(pong);
  uthread_start();

  exit();
}
