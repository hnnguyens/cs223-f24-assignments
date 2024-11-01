/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: November 1st, 2024
 * Description: Spawns processes according to the 
 * given diagram.
 ---------------------------------------------*/
#include <stdio.h>
#include <sys/types.h>  
#include <unistd.h>   
#include <stdlib.h> 

int main() {
  pid_t pid;

  printf("%d] A0\n", getpid()); //main process
  fflush(stdout); //forces printf to output 

  pid = fork(); 
  if (pid) { //if parent
    printf("%d] B0\n", getpid());
    fflush(stdout); 
    pid = fork();

    if (pid) {
      printf("%d] B1\n", getpid());
      printf("%d] Bye\n", getpid());
    }

    else {
      printf("%d] C1\n", getpid());
      printf("%d] Bye\n", getpid());
    }
  }
    
  else {
    printf("%d] C0\n", getpid());
    printf("%d] Bye\n", getpid()); 
  }

  return 0;
}
