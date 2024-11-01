/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: November 1st, 2024
 * Description: Creates a simple, customized 
 * shell. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>  
#include <readline/readline.h>
#include <readline/history.h>

#define RESET "\033[0m" //for colors
#define PERIWINKLE "\033[34m"
#define ORANGE "\033[33m" 

/**
 * Takes input from cmd line and executes commands through the child process.
*/
int main() {
  while (1) { //running process
    //distinct from others for storing info in the cmd line
    char *input = readline(PERIWINKLE "hnguyen5@goldnegate:/A08 🐒 " RESET); 
    char *args[1000]; 
    int idx = 0; //for indexing args 

    add_history(input); //reads the line from user input, and adds to history

    //for parsing
    char *token = strtok(input, " \n"); //end of the cmd line
      
    while (token != NULL) { //parsing tokens of cmd line
      args[idx] = token;
      idx++;
      token = strtok(NULL, " \n"); //to advance in string
    }

    args[idx] = NULL; //terminate list; all added in args 

    if (strcmp(args[0], "exit") == 0) {
      free(input);
      break;
    }

    //child process executes the commands; pass prompt into line 
    pid_t pid = fork(); 
    if (pid < 0) { 
      perror("fork failed!");
    } 
      
    else if (pid == 0) { //child process
      if (execvp(args[0], args) < 0) { //if command is invalid or unable to run
        perror(ORANGE "error! " RESET);
        exit(1); //exit child process if exec fails
      }
    } 
      
    else { 
      wait(NULL); //waits for child process to finish
    }

    free(input);
  }

  return 0;
}