/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: December 1st, 2024
 * Description: Uses N threads to build a binary
 * search tree of a given set of files. Gives the
 * user a prompt to find the dependencies of the
 * file by giving the filename.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include "tree.h"

//globals
pthread_mutex_t mutex; //for threading
struct tree_node *root = NULL; 

typedef struct{
  int thread_id;
  int num_files;
  char **files; //subset of files
} thread_data_t;


void *dependency(void *arg){
  thread_data_t *data = (thread_data_t *)arg; //cast generic

  for (int i = 0; i < data->num_files; i++) {
    struct tree_node *node = malloc(sizeof(struct tree_node));

    if (node == NULL) {
      printf("Malloc failed! \n");
      exit(0);
    }
    pthread_mutex_lock(&mutex);
    insert(data->files[i], node);
    pthread_mutex_unlock(&mutex);
  }
  
  return NULL;
}

int main(int argc, char *argv[]) {
  struct timeval tstart, tend; //for timer
  gettimeofday(&tstart, NULL);

  if (argc < 3){
    printf("usage: ./dependency <NumThreads> <Files>\n");
    return 0; //exit
  }

  //command line data
  int num_threads = atoi(argv[1]);
  char **files = &argv[2];  //using find
  int num_files = argc - 2; //is this right??

  if (num_threads <= 0) {
    printf("Number of threads needs to be > 0! \n");
    return 0; //exit
  }

  printf("Processing %d files\n", num_files);

  //initialization
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  thread_data_t *tdata = malloc(num_threads * sizeof(thread_data_t));
  pthread_mutex_init(&mutex, NULL); //initialize mutex

  int num = num_files / num_threads; //rough distribution
  int remainder = num_files % num_threads;
  int idx = 0;

  //file distribution
  for (int i = 0; i < num_threads; i++) {
    tdata[i].thread_id = i;
    tdata[i].files = &files[idx];

    tdata[i].num_files = num;

    if (i == (num_threads - 1))
    {
      tdata[i].num_files += remainder;
    }

    printf("Thread %d processing %d files (arguments %d to %d)\n", tdata[i].thread_id,
           tdata[i].num_files, idx, (idx + tdata[i].num_files));

    idx += tdata[i].num_files; //increment
  }

  //create threads
  for (int i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, dependency, &tdata[i]);
  }

  //join threads
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL);
  }

  //timer
  gettimeofday(&tend, NULL);
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec) / 1.e6;

  printf("Elapsed time is %.6f\n", timer);

  //UI here (interaction w/ list, filename, & quit)
  while (1) {  //while code is running
    char input[256]; //from cmd line

    printf("$ ");

    if (!fgets(input, 256, stdin) || strncmp(input, "quit", 4) == 0) { // if no input is entered
      break;
    }

    if (strncmp(input, "list", 4) == 0) {
      printSorted(root);
    }

    else {
      input[strcspn(input, "\n")] = '\0'; // whats the point of this line?
      struct tree_node *node = find(input, root);

      if (node)
      { // not null
        printf("%s has the following dependencies: \n", input);

        // file & find dependencies here
        FILE *file = fopen(node->data.name, "r");
        char line[1000];

        while (fgets(line, 1000, file))
        { // reading file
          if (strstr(line, "#include"))
          { // if found
            char dependency[100];
            char *start = strchr(line, '"');
            if (start)
            {
              char *end = strchr(start + 1, '"');
              int length = end - start - 1;
              strncpy(dependency, start + 1, length);
              dependency[length] = '\0';
              printf("%s\n", dependency);
            }

            else
            {
              char *start = strchr(line, '<');
              char *end = strchr(start + 1, '>');
              int length = end - start - 1;
              strncpy(dependency, start + 1, length);
              dependency[length] = '\0';
              printf("%s\n", dependency);
            }
          }
        }
      }
        else
        {
          printf("%s is not found \n", input);
        }
      }
    }

    // destroy & frees
    pthread_mutex_destroy(&mutex);
    clear(root);
    free(threads);
    free(tdata);

    return 0;
  }