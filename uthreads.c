#include "types.h"
#include "user.h"
#define NTHREAD        10  // maximum number of threads 


enum states{UNUSED, RUNNABLE, RUNNING}

struct tcb{
  int tid;
  int state;
  void *sp;
};

int nexttid; // next value for tid
int curthread; 
struct tcb threadtable[NTHREAD];
  

void uthread_init(){
  struct tcb *u;

  curthread = -1;
  nexttid = 0;

  for(u = threadtable; u < &threadtable[NTHREAD]; u++){
    u->tid = 0;
    u->state = UNUSED;
    u->sp = 0;
  }
}


void PUSH(abc){
  asm(
    "push abc;"
    "pop %eax;"
    "push %eax;"
    "subl %4, %esp;"
  )
}

void POP(){
  asm(
    "addl %4, %esp;"
  )
}

void uthread_create(void *func){
  struct thread *u;

  for(u = threadtable; u < &threadtable[NTHREAD]; u++){
    if(u->state == UNUSED){
      u->sp = malloc(4096);
      // Need to build stack
      // set up eip to func
      u->state = RUNNABLE;
      u->tid = nexttid++;
      return 0;
    }
  }
  printf(1,"Didn't find UNUSED entry");
  return -1;
}

void uthread_exit(){
  threadtable[curthread]->tid = 0;
  free(threadtable[curthread]->sp);
  threadtable[curthread]->sp = 0;
  threadtable[curthread]->state = UNUSED;
  uthread_scheduler();
}


void uthread_yield(){
  threadtable[curthread]->state = RUNNABLE;
  uthread_scheduler();
}


void uthread_scheduler(){
  int i = 0;
  int j = curthread;
  for(i = curthread + 1; i < NTHREAD; i++){
    if(threadtable[i]->state == RUNNABLE){
      threadtable[i]->state = RUNNING;
      curthread = i;
      // context switch occurs
    }
    // If found should never reach here
    i = 0;
    if(i = 0; i <= j; i++){
      if(threadtable[i]->state == RUNNABLE){
        threadtable[i]->state = RUNNING;
        curthread = i;
        // context switch occurs
      }
    }
  }
}

void ping(){
  printf(1,"ping\n");
  uthread_yield();
  printf(1,"ping\n");
  uthread_yield();
  printf(1,"ping\n");
  uthread_exit();
}


void pong(){
  printf(1,"pong\n");
  uthread_yield();
  printf(1,"pong\n");
  uthread_yield();
  printf(1,"pong\n");
  uthread_exit();
}


int main(){
  uthread_init();

  uthread_create(*func);
  uthread_create(*func);

  uthread_scheduler(); 


  exit();
}
