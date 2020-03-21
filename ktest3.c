#include "types.h"
#include "user.h"

int pid;

void
start_kthread(void* func){
  char* stack;

  stack = malloc(4096);
  kthread_fork(stack, func);
  return;
}

int
join_kthread(){
  void* free_stack;
  int pid = 0;

  pid = kthread_wait(&free_stack);
  free(free_stack);
  return pid;
}

void func1(){
  kill(pid);
  kthread_exit();
}

void func2(){
  exit();
}

int main(){
  pid = getpid();

  printf(1,"Kill Test: Thread trying to kill it's parent\n");
  start_kthread(func1);
  join_kthread();
  printf(1,"Kill Test Passed: Thread failed to kill it's parent\n");

  printf(1,"Exit Test: Thread trying to call exit() instead of thread_exit()\n");
  start_kthread(func2);
  join_kthread();
  printf(1,"Exit Test Passed: Thread got redirected to thread_exit()\n");

  exit();
}
