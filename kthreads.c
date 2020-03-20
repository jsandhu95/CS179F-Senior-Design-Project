#include "types.h"
#include "user.h"

void
start_kthread(void* func){
  char* stack;

  stack = malloc(4096);
  kthread_fork(stack, func);
  return;
}

int
close_kthread(){
  void* free_stack;
  int pid = 0;

  pid = kthread_wait(&free_stack);
  free(free_stack);
  return pid;
}

void func(){
  int i;
  for(i = 0; i < 10; i++){
    sleep(10);
  }
  kthread_exit();
}

int main(){
  int i = 0;
  int pid = 0;

  for(i = 0; i < 3; i++){
    printf(1,"Starting Thread #%d\n", i);
    start_kthread(func);
  }
  for(i = 0; i < 3; i++){
    pid = close_kthread();
    printf(1, "Exiting Thread with pid %d\n", pid);
  }
    exit();
}