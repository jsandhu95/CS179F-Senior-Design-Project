#include "types.h"
#include "user.h"

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

char*
start_kthread(){
  char* stack;
  stack = malloc(4096);
  return stack;
}

void
kthread_free(void* free_stack){
  free(free_stack);
  return;
}

void func(){
  int i;
  for(i = 0; i < 2; i++){
    sleep(10);
  }
  kthread_exit();
}


int main(){
  int i = 0;
  char* stack;
  void* free_stack;
  int pid = 0;

  void *(*fn)();
  fn = (void*)&func;

  for(i = 0; i < 50; i++){
    printf(1,"Starting Thread #%d\n", i);
    stack = start_kthread();
    kthread_fork(stack, fn);
  }
  for(i = 0; i < 50; i++){
    pid = kthread_wait(&free_stack);
    printf(1, "Exiting Thread with pid %d\n", pid);
    kthread_free(free_stack);
  }
    exit();
}
