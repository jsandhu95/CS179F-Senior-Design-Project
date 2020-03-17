#include "types.h"
#include "user.h"
#define NTHREAD        50  // Maximum number of threads 
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
int counter;
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
      // Go from bottom of stack to top of stack
      u->sp = u->stack + (STACK_SIZE/4) - 1;
      u->sp--;
      // Push return address of function for eip
      *u->sp = (uint)func;
      u->sp--;
      // Contents of u->sp becomes address of u->sp. Which is ebp
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
  // Read the first threads stack pointer and save it into both esp and ebp
  // Can't send it straight to the scheduler because there is no previous context to load
  asm("movl %0, %%esp; movl %%esp, %%ebp"
      :
      : "r" (threadtable[0].sp)
  );

  return;
}

void uthread_scheduler(){
  int i = 0;
  int index = 0;
  int curthread_temp = curthread; //Save the current thread so it can be accessed by assembler code

  for(i = 1; i <= NTHREAD; i++){
    index = (curthread + i) % NTHREAD; // Round Robin Scheduler. Go through all entries in threadtable starting at curthread and go back to beginning.

    if(threadtable[index].state == READY){
      curthread = index; // Curthread is current running thread

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
      // No additional code can be added here, Immediately using "return" is key to context switch functionality
      return;
    }
  }
  // This is how program calls exit()
  // When no threads in READY state can be found, program will exit here
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
  uthread_scheduler();
}


void ping(){
  for(int i = 0; i < 2; i++){
    counter++;
    printf(1,"ping #%d\n", counter);
    uthread_yield();
  }
  uthread_exit();
}


void pong(){
  for(int i = 0; i < 2; i++){
    counter++;
    printf(1,"pong #%d\n", counter);
    uthread_yield();
  }
  uthread_exit();
}


int main(){
  uthread_init();
  counter = 0;

  for(int i = 0; i < NTHREAD/2; i++){
    uthread_create(ping);
    uthread_create(pong);
  }
  uthread_start();

  // Shouldn't reach this point.
  // Once threads are created and started, they take over
  // Main process isn't used anymore
  exit();
}
