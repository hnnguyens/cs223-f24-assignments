/**
 * Exclaim class for Assignment 2.
 *
 * This program recieves input from the user and changes the lowercase letters
 * into characters from the char array exclaim, which consists of (@, #, !, *).
 *
 * @author Hazel Nguyen
 * @version 13 Sept 2024
 */

#include <stdio.h>
#include <stdlib.h> 
#include <time.h> //usedfor srand();

/**
 * Main code for Assignment 2.
 */
int main() {
    srand(time(NULL)); //so rand() isn't deterministic 
    char s[32]; 
    char exclaim[5] = {'@', '!', '#', '*', '\0'};
    int i = 0; //for indexing the input

    printf("%s", "Enter a word: ");
    scanf("%s", s); //takes user input

    while(s[i] != '\0') {
        if (s[i] >= 97 && s[i] <= 122) { //lowercase 
            s[i] = exclaim[rand() % 4];
        }	
        i++;
    }

    printf("%s%s\n", "OMG! ", s);

    return 0;
}
