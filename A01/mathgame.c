/***************************************************
 * mathgame.c
 * Author: Hazel Nguyen 
 * Implements a math game
 */

#include <stdio.h>
#include <stdlib.h> //for rand()
#include <time.h> //for srand()

int main() {
  int x; //rounds 
  int count = 0; 
  int y; //guess

  printf("%s", "Welcome to Math Game!\nHow many rounds do you want to play? ");
  scanf("%d", &x);

  //game loop
  for(int i = 0; i < x; i++) {
    srand(time(NULL)); //for generating a new time, not deterministic 
    int value = rand() % (9 + 1);
    int valueTwo = rand() % (9 + 1);
    printf("%d%s%d%s", value, " + ", valueTwo, " = ? ");
    scanf("%d", &y); //guess 

      if (y == (value+valueTwo)) {
	printf("%s", "\nCorrect!\n");
	count++;
      }	
      else {
	printf("%s", "\nIncorrect :(\n");
      }
  }

  printf("%s%d%s%d%s", "You answered ", count, "/", x, " correctly.\n");
  return 0;
}
