#include "types.h"
#include "user.h"

int pid;

void func1(){
  kill(pid);
  kthread_exit();
}

void func2(){
  exit();
}

int main(){
  char* stack1;
  char* stack2;
  void* free_stack1;
  void* free_stack2;
  pid = getpid();

  void *(*fn1)();
  fn1 = (void*)&func1;

  void *(*fn2)();
  fn2 = (void*)&func2;

  stack1 = malloc(4096);
  stack2 = malloc(4096);

    printf(1,"Kill Test: Thread trying to kill it's parent\n");
    kthread_fork(stack1, fn1);
    kthread_wait(&free_stack1);
    free(free_stack1);
    printf(1,"Kill Test Passed: Thread failed to kill it's parent\n");


    printf(1,"Exit Test: Thread trying to call exit() instead of thread_exit()\n");
    kthread_fork(stack2, fn2);
    kthread_wait(&free_stack2);
    free(free_stack2);
    printf(1,"Exit Test Passed: Thread got redirected to thread_exit()\n");




    exit();
}
