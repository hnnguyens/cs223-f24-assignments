/*----------------------------------------------
 * Author: Hazel Nguyen 
 * Date: 09/18/2024
 * Description: Turns a phrase into Ent speech. 
 * Asks for pause length & word, outputs Ent
 * phrase.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

int main() {
    int pause; 
    char word[32]; //in 32 char buffer
    printf("%s", "Pause length: ");
    scanf("%d", &pause);
    printf("%s", "Text: ");
    scanf("%s", word);

    int word_len = strlen(word); //grabs the length of the word
    char *ent_word;

    ent_word = malloc(sizeof(char) * (word_len + (pause * word_len)));

    if (ent_word == NULL) { //checks for space!
        printf("%s", "Not enough space!\n");
        return 1;
    }    

    int pos = 0; //for idx in ent_word
    int i = 0; //for loop, word idx
    while (word[i] != '\0') {
        ent_word[pos] = word[i];
        pos++;
        for (int x = 0; x < pause; x++) {
            ent_word[pos] = '.';
            pos++;
        }
        i++;
    }
    pos++;
    ent_word[pos] = '\0'; //terminating char
    printf("%s\n", ent_word);
    free(ent_word); //always free after malloc!

    return 0;
}
