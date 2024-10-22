/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: October 16th, 2024
 * Description: Reads a 64-bit unsigned integer 
 * and outputs it as a 8x8 1bpp sprite. 
 ---------------------------------------------*/
#include <stdio.h>

int main() {
  unsigned long img;
  scanf(" %lx", &img); //reads in hex values as an unsigned long
  printf("Image (unsigned long): %lx\n", img);
  
  //converts the values into an 8x8 bit sprite (64)
  for (int i = 1; i < 65; i++) { //row
      unsigned long mask = 0x1ul << (63 - i + 1); //bitmask
      unsigned long bit = (mask & img);

      if (bit) { //if != 0
        printf("1 ");
      }

      else {
        printf("0 "); 
      }
      
      if (i != 0 && i % 8 == 0) {
        printf("\n"); //for new line
      }
  }

  return 0;
}
