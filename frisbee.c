#include "types.h"
#include "user.h"

struct frisbee{
  int pass_number;
  int previous_thrower;
} frisbee;

int max_passes;
int num_players;


void play_frisbee(){
  int current_thrower = getpid();
  
  for(;;){
    if(frisbee.pass_number >= max_passes){
      kthread_exit();
    }
    if(current_thrower == frisbee.previous_thrower){
      sleep(10);
    }
    sem_wait();
    if(frisbee.pass_number < max_passes){
      printf(1, "Player with pid %d has caught the frisbee\n", current_thrower);
      frisbee.pass_number++;
      frisbee.previous_thrower = current_thrower;
      printf(1, "This is pass #%d\n", frisbee.pass_number);
      if(frisbee.pass_number >= max_passes){
        printf(1,"Max number of passes reached. Game over.\n");
      }
    }
    sem_signal();
  }
}


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

int main(int argc, char *argv[]){
  int i = 0;
  int pid = 0;
  frisbee.pass_number = 0;
  frisbee.previous_thrower = 0;

  if(argc == 3){
    num_players = atoi(argv[1]);
    max_passes = atoi(argv[2]);
    printf(1,"Number of players is %d and number of passes is %d\n", num_players, max_passes);
  }
  else if(argc > 3){
    printf(1,"Too many arguments\n");
    printf(1,"Command should be in the form \"frisbee [number of players 1-60] [number of passes]\"\n");
    exit();
  }
  else{
    printf(1,"Too few arguments\n");
    printf(1,"Command should be in the form \"frisbee [number of players 1-60] [number of passes]\"\n");
    exit();
  }

  sem_init();

  for(i = 0; i < num_players; i++){
    printf(1,"Starting Thread #%d\n", i);
    start_kthread((void*)play_frisbee);
  }
  for(i = 0; i < num_players; i++){
    pid = close_kthread();
    printf(1, "Exiting Thread with pid %d\n", pid);
  }
    exit();
}
