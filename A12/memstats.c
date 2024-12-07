/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: December 6th, 2024
 * Description: Prints information about total 
 * blocks, memory used, etc.
 ---------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

/**
 * Chunk struct. Contains size, the used amount of memory, and a ptr to the 
 * next in the list. 
*/
struct chunk {
  int size;
  int used;
  struct chunk *next;
};

/**
 * Print total number of memory blocks associated with malloc (currently in use
 * & free), along with the amount of underutilised memory as a %.
 * @param freelist the freelist in memory
 * @param buffer the used memory list
 * @param len the length of the buffer  
*/
void memstats(struct chunk* freelist, void* buffer[], int len) {
  int total = 0; //total blocks
  int memory = 0; //total memory 
  int usedBlocks = 0; 
  int freeBlocks = 0;
  int usedMemory = 0; 
  int freeMemory = 0;
  int used = 0; //used allocated memory 

  while (freelist != NULL) { //from freelist
    total++;
    freeBlocks++;
    memory += freelist->size; //add to total memory
    freeMemory += freelist->size;
    freelist = freelist->next; //increment freelist 
  }

  for (int i = 0; i < len; i++) { //from buffer
    if (buffer[i] != NULL) {
      struct chunk* ptr = (struct chunk*)((struct chunk*) buffer[i] - 1); //sentinel
      total++;
      usedBlocks++;
      memory += ptr->size; //add to total memory
      used += ptr->size;
      usedMemory += ptr->used;
    }
  }

  float underutilised = 1 - (float)usedMemory/used;
  printf("Total blocks: %d Freed blocks: %d Used blocks: %d\nTotal memory allocated: %d Free memory: %d Used memory: %d\nUnderutilised memory: %.2f\n", 
      total, freeBlocks, usedBlocks, memory, freeMemory, used, underutilised);  
}

/**
 * Main method.
 * @param argc the number of arguments 
 * @param argv the command line arguments
*/
int main (int argc, char* argv[]) {
  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
