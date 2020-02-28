#include "types.h"
#include "user.h"

void func(){
  exit();
}


int main(){
  int i = 0;
  char* stack;
  void* free_stack;

  void *(*fn)();
  fn = (void*)&func;

  stack = malloc(4096);

  for(i = 0; i < 1; i++){
    printf(1," Iteration ------- %d",i);
    kthread_fork(stack, fn);
  }
  for(i = 0; i < 1; i++){
    kthread_wait(&free_stack);
    free(free_stack);
  }
    exit();
}
