/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: November 10th, 2024
 * Description: Outputs a PPM image of the 
 * Mandelbrot set.
---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stddef.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char* argv[]) {
  struct timeval tstart, tend; //for timer
  gettimeofday(&tstart, NULL);

  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here !

  //allocate memory for image
  struct ppm_pixel *img = malloc((size*size) * sizeof(struct ppm_pixel)); 
  struct ppm_pixel *palette = malloc((maxIterations) * sizeof(struct ppm_pixel)); 
  srand(time(0)); //for randomizing the colors 

  //palette (assigning colors)
  for (int i = 0; i < maxIterations; i++) {
    palette[i].red = rand() % 255;
    palette[i].green = rand() % 255;
    palette[i].blue = rand() % 255;
  }

  //compute the mandelbrot set; using pseudo code from assignment description
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      float xfrac = (float) r / size;
      float yfrac = (float) c / size;
      float x0 = xmin + xfrac * (xmax - xmin);
      float y0 = ymin + yfrac * (ymax - ymin);

      float x = 0;
      float y = 0;
      int iter = 0;

      while (iter < maxIterations && x*x + y*y < 2*2) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < maxIterations) { 
        img[c * size + r] = palette[iter];  //write color to image at location (row, col)
      }

      else {
        img[c * size + r].red = 0;
        img[c * size + r].green = 0;
        img[c * size + r].blue = 0;
      }
    }
  }

  // compute image
  char fileName[64]; //assign name w/ timestamp here 

  //write the image to a PPM file
  sprintf(fileName, "mandelbrot-%d-%ld.ppm", size, time(0)); //formatting the filename
  write_ppm(fileName, img, size, size);
  gettimeofday(&tend, NULL); 
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec -tstart.tv_usec)/1.e6;

  printf("Computed mandelbrot set (%d x %d) in %.6f seconds\n", size, size, timer);
  printf("Writing file: %s\n", fileName);

  free(img);
  free(palette);
}
