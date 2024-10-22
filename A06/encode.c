/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: October 18th, 2024
 * Description: Encodes a message within a ppm file. 
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"
#define MAX_LEN 32

/**
 * Main method containing operations for encoding a message within a file.
 * @param argc int containing the number of arguments in cmd line
 * @param argv a pointer to the items in the cmd line
 * @return 0 if successful 
*/
int main(int argc, char** argv) {
  if (argc != 2) {
    printf("Please enter a ppm file to encode!\n");
    printf("usage: encode <file.ppm>\n");
    return 0;
  }

  //pixel struct the size of w & h using read_ppm
  int w, h;
  struct ppm_pixel* pixel = read_ppm(argv[1], &w, &h);  
  unsigned char* pixelppt = (unsigned char*) pixel; //for operations
  int totalPixels = w * h; //for looping & grid operations 

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters in the image: %d", ((totalPixels * 3)/8)-1); 

  char message[100]; 
  int len = strlen(argv[1]);
  printf("\nEnter a message: ");
  scanf("%c\n", message);
    
  //ENCODE HERE
  unsigned char mask = 0b1;
  unsigned char bit;
  unsigned char color; //the new color to be assigned

  for (int i = 0; i < totalPixels; i++) {
    bit = pixelppt[i] & mask; //masked

    if (i >= strlen(message)) { //if exceeded
      bit = 0;
    }

    else {
      bit = (message[i] >> (7 - i)) & 0b1; //right shift & masking
    }

    color = (pixelppt[i] & ~mask) | bit; //adjusts new color value
    pixelppt[i] = color;
  }

  char outputFilename[MAX_LEN + 15]; // +15 for "-encoded.ppm" and '\0'
  snprintf(outputFilename, sizeof(outputFilename), "%s-encoded.ppm", argv[1]);

  printf("Writing file %s\n", outputFilename);

  free(pixel); //free memory

  return 0;
}

