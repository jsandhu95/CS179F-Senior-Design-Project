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

struct semaphore{
  int lock;
  void (*acquire)(const struct semaphore*);
  void (*release)(const struct semaphore*);
//  void* acquire;
//  void* release;
};

int nexttid;                            // Next value for tid
int curthread;                          // Current running thread 
int counter;
struct thread threadtable[NTHREAD];     // Thread table
struct semaphore sem;
  

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

void acquire_lock(const struct semaphore* sem){
  while(sem->lock != 0){
  }

  sem->lock = 1;
  return;
}

void release_lock(const struct semaphore* sem){
  sem->lock = 0;
  return;
}

void sem_init(){
  struct semaphore *s;
  s = &sem;
  s->lock = 0;
  s->acquire = acquire_lock;
  s->release = release_lock;
//  sem.lock = 0;
  counter = 0;
//  void (*acquire_ptr)(int) = &acquire_lock;
//  void (*release_ptr)(int) = &release_lock;
//  sem.acquire = acquire_ptr;
//  sem.release = release_ptr;
}


void lock(){
  //Acquire lock
  sem.acquire(&sem);
  printf(1,"Thread #%d has acquired the lock\n", threadtable[curthread].tid);
  counter++;
  printf(1,"Value of the shared variable is currently %d\n", counter);
  //Release lock
  sem.release(&sem);
  uthread_exit();
}



int main(){
  uthread_init();
  sem_init();

  uthread_create(lock);
  uthread_create(lock);
  uthread_create(lock);
  uthread_start();

  exit();
}
