/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: October 3rd, 2024
 * Description: A short test that reads in 
 * feep_raw.ppm and then writes it to a file 
 * test.ppm by calling the function. Prints
 * the contents.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

/**
 * Main method. Tests write_ppm.c
 * @param argc the number of cmd line args
 * @param argv pointer to an array of characters listing all args 
*/
int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  // test writing the file to test.ppm, reload it, and print the contents
  write_ppm("test.ppm", pixels, w, h);
  free(pixels); //need to free before next read

  pixels = read_ppm("test.ppm", &w, &h); //reads and assigns it to pixels

  printf("Testing file feep-raw.ppm: 4 4\n");
  //print
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      printf("(%d, %d, %d) ", pixels[i * h + j].red, pixels[i * h + j].green, 
        pixels[i * h + j].blue);
    }
    printf("\n");
  }

  free(pixels); //free memory
  return 0;
}
