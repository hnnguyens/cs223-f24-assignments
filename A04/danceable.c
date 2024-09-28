/*----------------------------------------------
 * Author: Hazel Nguyen 
 * Date: 09/26/24
 * Description: Allows users to easily access the most danceable songs, storing
 * the songs into a linked list. 
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Creates a typedef struct song.
 */
typedef struct song {
    char title[128];
    char artist[128];
    int durationMin; //parsed from ms
    int durationSec;
    float danceability;
    float energy;
    float tempo;
    float valence; //musical positivity level

} song;

/**
 * Creates a typedef struct node.
 */
typedef struct node {
    song data;
    struct node* next; //pointer 

} node;

/**
 * Prints the linked list of songs into an table.
 * @param node the linked list
 */
void print(node* node) {
    int num = 0;

    while (node != NULL) {
        if (node->data.durationSec < 10) {
            printf("%d) %-30s %-20s (%d:0%ds) D: %.3f E: %.3f T: %.3f V: %.3f\n",
                    num, node->data.title,
                    node->data.artist,
                    node->data.durationMin,
                    node->data.durationSec,
                    node->data.danceability,
                    node->data.energy,
                    node->data.tempo,
                    node->data.valence);
        }
        else{
            printf("%d) %-30s %-20s (%d:%ds) D: %.3f E: %.3f T: %.3f V: %.3f\n",
                    num, node->data.title,
                    node->data.artist,
                    node->data.durationMin,
                    node->data.durationSec,
                    node->data.danceability,
                    node->data.energy,
                    node->data.tempo,
                    node->data.valence);
        }
        num++;
        node = node->next;
    }
    printf("\nDataset contains %d songs\n", num);
    printf("=======================\n");
}

/**
 * Finds and returns the most danceable song in the list, null if empty.
 * @param current the head node / linked list
 */
node* mostDanceable(node *current) {
    if (current == NULL) { //check for empty list 
        return NULL;
    }

    float danceable = current->data.danceability; 
    node* mostDanceable = current;

    mostDanceable = current; //represents the most danceable song
    current = current->next;
    while (current != NULL) {
        if (current->data.danceability > danceable) {
            danceable = current->data.danceability; //max alg
            mostDanceable = current;
        }
        current = current->next;
    }
    return mostDanceable; //most danceable 
}

/**
 * Removes a song from the linked list.
 * @param head a pointer to the front of the linked list
 * @param data an indication of which node to delete
 */
void removeSong(node** head, song data) {
    node* dummy = (*head)->next;
    node* prev = *head; //start of the list 

    if (strcmp(prev->data.title, data.title) == 0) {
        *head = dummy;
        free(prev);
        return;
    }

    while(dummy != NULL) {
        if (strcmp(dummy->data.title, data.title) == 0) {
            prev->next = dummy->next;
            free(dummy);
            return;
        }
        prev = prev->next;
        dummy = dummy->next;
    }

}

/**
 * Prints the most danceable song, calling mostDanceable(), remove() and print()
 * in the process.
 * @param head the pointer reference to the front of the list. 
 */
void printDanceable(node* head) {
    char key;
    int dance = 0;

    while (dance == 0) {
        printf("\nPress 'd' to show the most danceable song (any other key to quit): ");
        scanf(" %c", &key);

        if (key == 'd') {
            node* jam = mostDanceable(head); //finds the most danceable 
            if (jam == NULL) {
                printf("Dataset contains 0 songs\n");
            }
            else {
                printf("\n----------------------------------------- Most danceable -----------------------------------------\n");
                if (jam->data.durationSec < 10) {
                    printf("%-30s %-20s (%d:0%ds) D: %.3f E: %.3f T: %.3f V: %.3f\n",
                            jam->data.title,
                            jam->data.artist,
                            jam->data.durationMin,
                            jam->data.durationSec,
                            jam->data.danceability,
                            jam->data.energy,
                            jam->data.tempo,
                            jam->data.valence);
                }   
                else {
                    printf("%-30s %-20s (%d:%ds) D: %.3f E: %.3f T: %.3f V: %.3f\n",
                            jam->data.title,
                            jam->data.artist,
                            jam->data.durationMin,
                            jam->data.durationSec,
                            jam->data.danceability,
                            jam->data.energy,
                            jam->data.tempo,
                            jam->data.valence);
                }
                printf("--------------------------------------------------------------------------------------------------\n");

                removeSong(&head, jam->data); //removes most danceable 
                print(head);
            }
        }
        else {
            break;
        }
    }
}

/**
 * Inserting a song into the front of the linked list. 
 * @param data the song information to be added to the node.
 * @param head a pointer to the front of the current list.
 */
node* insert(song* data, node* head) {
    node* n = malloc(sizeof(node)); //pointer for the nodes in the list

    if (n == NULL) { //malloc failed
        printf("ERROR: Out of space!");
        exit(1); //quits program
    }

    n->data = *data;
    n->next = head; 

    return n;
}

int main() {
    FILE *inFile; //csv file to be parsed

    inFile = fopen("songlist.csv", "r"); 
    if (inFile == NULL) {
        printf("%s", "Error: Invalid file!\n");
        return 1; // meaning that the code terminates w/ an error
    }

    char line[129]; //char buffer (check length)
    fgets(line, sizeof(line), inFile); //first line, header

    node *songList = NULL; //creates LL & initialize to null
    char* token; //token pointer

    while (fgets(line, sizeof(line), inFile) != NULL) {
        int pos = 0; //position in line (for tokenizing)
        token = strtok(line, ",");

        song* input = (song*) malloc(sizeof(song));

        if (input == NULL) {
            printf("Error! No memory space\n");
            return 1;
        }
        while (token != NULL) {
            if (pos == 0) { //title
                strcpy(input->title, token); 
            }

            if (pos == 1) { //artist
                strcpy(input->artist, token);
            }

            if (pos == 2) { //duration
                int duration = atoi(token);
                input->durationMin = (duration/1000)/60;
                input->durationSec = (duration/1000)%60;
            }

            if (pos == 3) { //danceability 
                input->danceability = atof(token);
            }

            if (pos == 4) { //energy
                input->energy = atof(token);
            }

            if (pos == 5) { //tempo
                input->tempo = atof(token);
            }

            if (pos == 6) { //valence
                input->valence = atof(token);
            }

            token = strtok(NULL, ","); //advances in string
            pos++;
        }
        songList = insert(input, songList); 
        free(input); //frees temporary input after insertion (avoid duplicates)
    }

    print(songList); //newNode should be the head after the last input
    printDanceable(songList); //loop until quit
    fclose(inFile); //close file

    return 0;
}
