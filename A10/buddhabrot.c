/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: November 15th, 2024
 * Description: Outputs a PPM image of a 
 * Buddhabrot using multiple threads. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <pthread.h>
#include <math.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

/**
 * Global variables, mutex and barrier. 
*/
pthread_mutex_t mutex; 
pthread_barrier_t barrier; //for syncing the threads 

/**
 * Thread struct. 
*/
struct thread_data {
  pthread_t tid; //thread id

  //private variables 
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int numProcesses;

  int start_idx_row; 
  int end_idx_row;
  int start_idx_col;
  int end_idx_col;

  //shared variables
  int *membership; //boolean array; 1 false 0 true 
  int *countArr;
  struct ppm_pixel *img; //image
};

/**
 * Thread method buddhabrot. Contains steps 1-3 for computing on the quadrant
 * of the image. 
 * @param arg the generic type 
 * @return the thread data 
*/
void *buddhabrot(void *arg) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int maxCount = 0;

  struct thread_data *data = (struct thread_data*) arg; //typecast the generic 
  data->tid = pthread_self(); //method for finding tid

  printf("Thread %lu) sub-image block: cols (%d, %d) to rows (%d, %d)\n", 
    (unsigned long) data->tid, data->start_idx_row, data->end_idx_row, 
      data->start_idx_col, data->end_idx_col);

  //Step one: determine whether it's in a mandelbrot set 
  for (int r = data->start_idx_row; r < data->end_idx_row; r++) { //each row
    for (int c = data->start_idx_col; c < data->end_idx_col; c++) { //each col
      float xfrac = r / (float) data->size;
      float yfrac = c / (float) data->size;
      float x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      float y0 = data->ymin + yfrac * (data->ymax - data->ymin);
      float x = 0;
      float y = 0;
      float iter = 0;

      while (iter < data->maxIterations && x*x + y*y < 2*2) {
        float xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        iter++;
      }

      if (iter < data->maxIterations) { //escaped 
        data->membership[r * size + c] = 0; 
      }

      else {
        data->membership[r * size + c] = 1; 
      }
    }
  }

  //Step 2: if escape: update counts for each x,y coordinate visited 
  for (int r = data->start_idx_row; r < data->end_idx_row; r++) {
    for (int c = data->start_idx_col; c < data->end_idx_col; c++) {
      if (data->membership[r * data->size + c] == 1) { //if in mandelbrot set 
        continue;
      }
        float xfrac = r / (float) data->size;
        float yfrac =  c / (float) data->size;
        float x0 = data->xmin + xfrac * (data->xmax - data->xmin);
        float y0 = data->ymin + yfrac * (data->ymax - data->ymin);
        float x = 0;
        float y = 0;

        while (x*x + y*y < 2*2) {
          float xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;

          int yrow = round(data->size * (y - data->ymin)/(data->ymax - data->ymin));
          int xcol = round(data->size * (x - data->xmin)/(data->xmax - data->xmin));

          if (yrow < 0 || yrow >= data->size) continue; // out of range
          if (xcol < 0 || xcol >= data->size) continue; // out of range

          //increment count at (yrow, xcol)
          pthread_mutex_lock(&mutex); //lock for critical section; counting
          
          data->countArr[yrow * size + xcol] += 1; //increment 
          
          if(data->countArr[yrow * data->size + xcol] > maxCount) { //update count
            maxCount = data->countArr[yrow * data->size + xcol]; 
          }

          pthread_mutex_unlock(&mutex);
        }
    }
  }

  pthread_barrier_wait(&barrier); //wait before color computing
  
  //Step three: turn counts into a color 
  float gamma = 0.681;
  float factor = 1.0/gamma;

  for (int r = data->start_idx_row; r < data->end_idx_row; r++) {
    for (int c = data->start_idx_col; c < data->end_idx_col; c++) {
      float value = 0;


      if (data->countArr[r * data->size + c] < 0) {
        value = log(data->countArr[r * data->size + c] / log(maxCount));
        value = pow(value, factor);
      }
      
      //assign colors
      data->img[r * data->size + c].red = value * 255;
      data->img[r * data->size + c].green = value * 255;
      data->img[r * data->size + c].blue = value * 255;
    }
  }

  return (void *) NULL;
}

/**
 * Main method. 
 * @param argc the number of arguments
 * @param argv the command line 
 * @return 0 on success
*/
int main(int argc, char* argv[]) {
  struct timeval tstart, tend; //for timer
  gettimeofday(&tstart, NULL);
  srand(time(0));

  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;

  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // compute image
  struct ppm_pixel *img = malloc((size*size) * sizeof(struct ppm_pixel)); 
  int *membership = malloc((size * size) * sizeof(int));
  int *countArr = malloc((size * size) * sizeof(int));

  for (int i = 0; i < size*size; i++) {
    countArr[i] = 0;
    membership[i] = 0;
  }

  pthread_mutex_init(&mutex, NULL);
  pthread_barrier_init(&barrier, NULL, 4); //for 4 threads 

  pthread_t threads[numProcesses]; //4 threads for 4 quadrants
  struct thread_data data[numProcesses];
  int quadrant = size / 2;
  pthread_t thread_id = pthread_self(); 

  //creates threads
  for (int i = 0; i < numProcesses; i++) {
    int row_block = i / 2;
    int col_block = i % 2;
    data[i].size = size;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIterations = maxIterations;
    data[i].start_idx_row = row_block * quadrant;
    data[i].end_idx_row = (row_block + 1) * quadrant;
    data[i].start_idx_col = col_block * quadrant;
    data[i].end_idx_col = (col_block + 1) * quadrant;
    data[i].img = img;
    data[i].membership = membership;
    data[i].countArr = countArr;

    pthread_create(&threads[i], NULL, buddhabrot, (void *)&data[i]);
  }

  //join threads to finish 
  for (int i = 0; i < numProcesses; i++) {
    pthread_join(threads[i], NULL); 
  }

  //print completion status
  for (int i = 0; i < numProcesses; i++) {
    printf("Thread %lu) finished\n", (unsigned long)data[i].tid);
  }

  //print statements 
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  //for timer 
  gettimeofday(&tend, NULL); 
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec -tstart.tv_usec)/1.e6;
  printf("Computed buddhabrot set (%dx%d) in %.6f seconds\n", size, size, timer);

  char fileName[100];
  sprintf(fileName, "buddhabrot-%d-%ld.ppm", size, time(0)); //format filename
  write_ppm(fileName, (struct ppm_pixel *) img, size, size);

  printf("\nWriting file: %s", fileName);

  pthread_mutex_destroy(&mutex); //destroy after use in buddhabrot function
  pthread_barrier_destroy(&barrier); //after step 2 in buddhabrot

  free(img);
  img = NULL;
  free(membership);
  membership = NULL;
  free(countArr);
  countArr = NULL;

  return 0;
}
