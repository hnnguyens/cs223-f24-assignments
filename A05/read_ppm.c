/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: October 3rd, 2024
 * Description: Reads a PPM file stored in binary format.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

//Choose *one* to implement (do not remove the other one!)

/**
 * Reads a ppm binary file and turns it into a flat array.
 * @param filename the ppm file to reac
 * @param w a pointer to width
 * @param h a pointer to height 
 * @return struct containing the pixel map, exit if invalid file 
*/
struct ppm_pixel* read_ppm(const char* filename, int* w, int* h) { 
  //takes filename as input
  FILE *infile = fopen(filename, "r");
  char buffer[100]; 
  int maxValue; //255

  if (infile == NULL) {
    printf("Invalid file\n");
    exit(1);
  }

  //skips headers
  fgets(buffer, sizeof(buffer), infile); //gimp
  fgets(buffer, sizeof(buffer), infile); //plug-in

  fscanf(infile, "%d %d\n %d\n", w, h, &maxValue); //reads data from stream

  //pixel struct the size of w & h
  struct ppm_pixel* pixel = malloc(sizeof(struct ppm_pixel) * (*w * *h)); 

  if (pixel == NULL) { //if no space; user responsible for free
    printf("Memory cannot be allocated\n");
    exit(1);
  }

  //reads & gets the content
  fgets((char *)pixel, sizeof(struct ppm_pixel) * (*w * *h), infile); 

  fclose(infile); //no memory leak

  return pixel;
}

/**
 * Reads a ppm binary file and turns it into a 2D array.
 * @param filename the ppm file to reac
 * @param w a pointer to width
 * @param h a pointer to height 
 * @return struct containing the pixel map, exit if invalid file 
*/
struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) { //2D 
  return NULL;
}
