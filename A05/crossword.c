/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: October 3rd, 2024
 * Description: Generates a crossword taking two
 * words from the user.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Prints the crossword out.
 * @param twoArr a double pointer to the 2D array
 * @param rows the number of rows
 * @param cols the number of columns 
 * @return void
*/
void printCrossword(char **twoArr, int rows, int cols) {
    for (int r = 0; r < rows; r++) {
      for (int c = 0; c < cols; c++) {
         printf(" %c", twoArr[r][c]); //prints the entire row
      }
      printf("\n");
    }
}

/**
 * Main method.
 * @param argc stores the # of cmd line args (including program name)
 * @param argv pointer to an array of characters listing all args 
 * @return int 0 if successful
*/
int main(int argc, char *argv[]) { //parameters for reading from cmd line
  if (argc < 2 && argc > 3) { //if cmd line is empty or >2 words
    printf("Please provide two words to make a crossword!\n"); 
    return 0; //quit
  }

  char *wordOne = argv[1];
  char *wordTwo = argv[2]; 

  int idxRow; //intersect here
  int idxCol = -1;

  for (int i = 0; i < strlen(wordOne); i++) { //goes through letters of wordOne
    for (int j = 0; j < strlen(wordTwo); j++) {
      if (wordOne[i] == wordTwo[j]) { //if equivalent, check syntax here
          idxCol = j;
          idxRow = i;
          break; //does this break through all loops
      }
    }
  }

  if (idxCol == -1) { //cannot make crossword
    printf("No common letter!\n");
    return 0; //quit
  }

  //create 2D array using method 2
  int cols = strlen(wordTwo);
  int rows = strlen(wordOne);
  char **twoArr = malloc(sizeof(char*) * rows); 

  for (int i = 0; i < rows; i++) {
    twoArr[i] = malloc(sizeof(char) * (cols + 1)); //allocates data for the cols
  }

  //add values to 2D array
  for (int r = 0; r < rows; r++) { 
    for (int c = 0; c < cols; c++) { 
      if (c == idxCol) { //intersect 
        twoArr[r][c] = wordOne[r];
      }
      else if (r == idxRow) {
        twoArr[r][c] = wordTwo[c];
      }
      else {
        twoArr[r][c] = '*';
      }
    }
    twoArr[r][cols] = '\0'; //terminating char
  }

  printCrossword(twoArr, rows, cols);

  //free all values at the end; in reverse order 
  for (int i = 0; i < rows; i++) {
    free(twoArr[i]);
  }
  free(twoArr);

  return 0;
}