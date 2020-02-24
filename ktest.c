#include "types.h"
#include "user.h"

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
    stack = malloc(4096);
    kthread_fork(stack, fn);
  }
  for(i = 0; i < 50; i++){
    pid = kthread_wait(&free_stack);
    printf(1, "Exiting Thread with pid %d\n", pid);
    free(free_stack);
  }
    exit();
}
