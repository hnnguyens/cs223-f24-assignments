/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: December 6th, 2024
 * Description: Malloc and free implementation based
 * off of the reading. Added a "used" parameter to 
 * the implementation. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Chunk struct. 
*/
struct chunk {
  int size; //size of memory allocated from sbrk 
  int used; //amount currently in use
  struct chunk *next;
};

struct chunk *flist = NULL;

/**
 * Malloc implementation. 
 * @param size the size in memory
 * @return void 
*/
void *malloc (size_t size) {
  // TODO: Implement malloc with a free list (See reading for details)
  if (size == 0) {
    return NULL;
  }
  
  struct chunk *next = flist;
  struct chunk *prev = NULL;

  while (next != NULL) {
    if (next->size >= size) {
      if (prev != NULL) {
        prev->next = next->next;
      }
      else {
        flist = next->next;
      }
      next->used = size;
      return (void *)(next +1); 
    }
    else {
      prev = next;
      next = next->next;
    }
  }

  //using sbrk to allocate new memory 
  void *memory = sbrk(size + sizeof(struct chunk));

  if (memory == (void *) -1) {
    return NULL;
  }

  else {
    struct chunk *cnk = (struct chunk*) memory;
    cnk->size = size;
    cnk->used = size;
    return (void*)(cnk +1);
  }
}

/**
 * Free implementation.
 * @param memory to be freed
 * @return void 
*/
void free(void *memory) {
  // TODO: Implement malloc with a free list (See reading for details)
  if (memory != NULL) { //jumping back one chunk position
    struct chunk *cnk = (struct chunk*)((struct chunk*)memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}

