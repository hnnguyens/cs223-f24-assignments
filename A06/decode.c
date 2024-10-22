/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: October 16th, 2024
 * Description: Reads a PPM file and outputs the 
 * message hidden in the bits of color. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

/**
 * Places the byte into message if 8 bits have been collected, ensures removal 
 * of redundant code.
 * @param message a pointer to the message
 * @param messageIdx a pointer to the index
 * @param currentByte the value of byte
 * @param bits the number of bits
 * @param maxChars a counter
 * @return void
 */
void storeByte(unsigned char* message, int* messageIdx, unsigned char* currentByte, int* bits, int* maxChars) {
  if (*bits == 8) {
    message[*messageIdx] = *currentByte; //the hidden ASCII char
    (*messageIdx)++; //() syntax for dereferencing 
    *currentByte = 0; //reset
    *bits = 0; //reset 
    (*maxChars)++;
  }
}

/**
 * Extracting and parsing the bits; for each color in the image; continues 
 * until \0
 * @param pixels the parsed pixels
 * @param totalPixels total number of pixels
 * @return void
 */
void hiddenMessage(struct ppm_pixel* pixels, int totalPixels) {
  int bits = 0; //once 8, reset
  int maxChars = 0; 
  int messageIdx = 0; //to add to message[] string
  unsigned char message[100] = {0}; //buffer for the hidden message. 
  unsigned char currentByte = 0; //0 or 1; unsigned because pixels deal w/ unsigned chars 

  for (int i = 0; i < totalPixels; i++) {
    for (int j = 0; j < 3; j++) { //for RGB
      unsigned char bit;

      //extracting the LSB, based on color
      if (j == 0) {
        bit = pixels[i].red & 1; //0 or 1 
      } 
      else if (j == 1) {
        bit = pixels[i].green & 1;
      }     
      else {
        bit = pixels[i].blue & 1;
      }
      //combine w/ currentByte
      currentByte = (currentByte << 1) | bit;
      bits++;
      
      //check
      storeByte(message, &messageIdx, &currentByte, &bits, &maxChars);
    }
  }

  printf("%d\n", maxChars); 
  message[messageIdx] = '\0'; //terminating char 
  printf("%s\n", message); 
}

/**
 * Main method. Takes info from cmd line.
 * @param argc the number of args
 * @param argv cmd line args
 * @return 0 if successful
*/
int main(int argc, char** argv) {
  if (argc != 2) { //if the cmd line is empty 
    printf("Please input a file to decode!\n");
    printf("usage: decode <file.ppm>\n");
    exit(1);
  }

  //pixel struct the size of w & h, using read_ppm
  int w, h;
  struct ppm_pixel* pixel = read_ppm(argv[1], &w, &h);
  int totalPixels = w * h; //for looping & grid operations 

  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  printf("Max number of characters within this image: ");
   
  hiddenMessage(pixel, totalPixels); 

  free(pixel);
  return 0;
}

