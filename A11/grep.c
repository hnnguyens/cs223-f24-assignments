/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: December 1st, 2024
 * Description: Implements a program using N 
 * threads to search for a keyword in a set of
 * files.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>

//globals
pthread_mutex_t mutex; 

/**
 * Typedef struct for thread data type.
 * @param thread_id the identifier
 * @param num_files number of files the thread will search
 * @param count for # of times found
 * @param files a subset from source files
 * @param keyword the word to be searched
 */
typedef struct {
  int thread_id;
  int num_files;
  int count;
  char **files; 
  char *keyword; 
} thread_data_t;

/**
 * The thread search method, finds the specified word using strstr and adds it 
 * to count.
 * @param arg generic type
 * @return nothing, void
*/
void *search(void *arg) {
  thread_data_t *data = (thread_data_t *)arg; //cast generic
  char line[1000]; //buffer

  for (int i = 0; i < data->num_files; i++) {
    FILE *file = fopen(data->files[i], "r"); //reading from data struct files

    if (!file) { //if file is invalid, corrupt, etc
      printf("Error opening file!\n");
      exit(0);
    }

    //read file
    while (fgets(line, 1000, file)) {
      if (strstr(line, data->keyword)) { //if found
        pthread_mutex_lock(&mutex);
        printf("%d) %s: %s", data->thread_id, data->files[i], line); 
        pthread_mutex_unlock(&mutex);
        data->count++;
      }
    }

    fclose(file); 
  }

  return NULL;
}

/**
 * Main method. Parses from cmd line, creates threads and keeps track of 
 * elapsed time.
*/
int main(int argc, char *argv[]) {
  struct timeval tstart, tend; //for timer
  gettimeofday(&tstart, NULL);

  if (argc < 3) { //if no arguments, exit
    printf("usage: ./grep <NumThreads> <Keyword> <Files>\n"); 
    return 0; //exit
  }

  //command line data 
  int num_threads = atoi(argv[1]);
  char *keyword = argv[2]; 
  char **files = &argv[3]; //double pointer because its an array of files
  int num_files = argc - 3; //3 because (# of threads, keyword, sources)

  if (num_threads <= 0) {
    printf("Number of threads must be greater than one!\n");
    return 0; //exit
  }

  printf("Searching %d files for keyword: %s\n", num_files, keyword);

  //initialization
  pthread_t *threads = malloc(num_threads * sizeof(pthread_t));
  thread_data_t *tdata = malloc(num_threads * sizeof(thread_data_t));
  pthread_mutex_init(&mutex, NULL); 

  int num = num_files / num_threads; //rough distribution
  int remainder = num_files % num_threads; 
  int idx = 0; //for files

  //redistribute source files
  for (int i = 0; i < num_threads; i++) {
    tdata[i].thread_id = i; 
    tdata[i].files = &files[idx]; 

    tdata[i].num_files = num; 

    //if it's the last thread
    if (i == (num_threads - 1)) {
      tdata[i].num_files += remainder;
    }

    tdata[i].keyword = keyword;
    tdata[i].count = 0;

    printf("Thread %d searching %d files (%d to %d)\n", tdata[i].thread_id, 
      tdata[i].num_files, idx, (idx + tdata[i].num_files));

    idx += tdata[i].num_files; //increment 
  }

  //create threads
  for (int i = 0; i < num_threads; i++) {
    pthread_create(&threads[i], NULL, search, &tdata[i]);
  }

  //wait for threads to finish, then join
  for (int i = 0; i < num_threads; i++) {
    pthread_join(threads[i], NULL); 
  }

  gettimeofday(&tend, NULL); 
  float timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec -tstart.tv_usec)/1.e6;

  printf("Elapsed time is %.6f\n", timer); 
  
  for (int i = 0; i < num_threads; i++) {
    printf("Thread %d found %d line(s) containing keyword: %s\n", i, 
      tdata[i].count, tdata[i].keyword);
  }

  //destroy & frees
  pthread_mutex_destroy(&mutex);
  free(threads);
  free(tdata);

  return 0;
}
