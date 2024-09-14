/**
 * Acronym code for Assignment 2.
 *
 * Computes an acronym for a given phrase. Creates a string of all capital 
 * letters from the given phrase. 
 *
 * @author Hazel Nguyen
 * @version 13 Sept 2024
 */

#include <stdio.h>

/**
 * Main code for Assignment 2.
 */
int main() {
    char phrase[1024]; 
    int i = 0; //idx
    char ans[1024]; 
    int pos = 0;

    printf("%s", "Enter a phrase: ");
    scanf("%[^\n]%*c", phrase); //allows for spaces

    while (phrase[i] != '\0') {
        if (phrase[i] >= 65 && phrase[i] <= 90) { //uppercase
            ans[pos] = phrase[i];
            pos++;
        }
        i++;
    }

    ans[pos] = '\0'; //adds terminating char

    printf("%s%s\n", "Your acronym is ", ans);
    return 0;
}
