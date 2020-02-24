#include "types.h"
#include "user.h"

int
start_thread(void* fn)
{
  char* stack;

  stack = malloc(4096);
  kthread_fork(stack, fn);
  return 0;
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
  void* stack;
  int pid = 0;

  void *(*fn)();
  fn = (void*)&func;

  for(i = 0; i < 50; i++){
    printf(1,"Starting thread #%d\n",i);
    start_thread(fn);
  }
  for(i = 0; i < 50; i++){
    pid = kthread_wait(&stack);
    printf(1, "Exiting thread with pid %d\n", pid);
    free(stack);
  }
    exit();
}
