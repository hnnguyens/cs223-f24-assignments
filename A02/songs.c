/**
 * Songs program for Assignment 2. 
 * 
 * Fills an array with three or more songs and then allows the user to edit the
 * data in the array. 
 * 
 * @author: Hazel Nguyen
 * @version: 13 Sept 2024
 */

#include <stdio.h>
#include <string.h>

/**
 * Creates a typedef struct song.
 */
typedef struct song {
    int durationMin;
    int durationSec;
    float danceability;
    char artist[32];
    char title[32];
} song; 

int main() {
    int id; //index of the song in list  
    song one, two, three; 

    /**
     * Prints the struct song.
     * @param songList The list of the songs to be printed
     * @return void
     */
    void toString(song songList[]) {
        for (int i = 0; i < 3; i++) {
            printf("%d) %-20s artist: %-20s duration: %d:%d danceability: %.2f\n", 
                    i, songList[i].title, 
                    songList[i].artist, 
                    songList[i].durationMin,
                    songList[i].durationSec, 
                    songList[i].danceability);
        }
        printf("%s\n", "**********************");
    }

    strcpy(one.title, "On Melancholy Hill");
    strcpy(two.title,"Heartbeat");
    strcpy(three.title, "Breakin' Dishes"); 

    strcpy(one.artist, "Gorillaz");
    strcpy(two.artist, "Childish Gambino");
    strcpy(three.artist, "Rihanna");

    one.durationMin = 3;
    one.durationSec = 53;
    two.durationMin = 4;
    two.durationSec = 29;
    three.durationMin = 3;
    three.durationSec = 20;

    one.danceability = 0.6;
    two.danceability = 0.5;
    three.danceability = 0.9;

    song songList[3];
    songList[0] = one;
    songList[1] = two;
    songList[2] = three; 

    printf("%s\n", "Welcome to Hazel Nguyen's Song List.");
    toString(songList); 

    printf("%s", "Enter a song ID that you wish to edit [0, 1, 2]: ");
    scanf("%d", &id);

    if (id > 3 || id < 0) { //checks for correct input 
        printf("%s\n", "Invalid choice!");
        return 0; //quit program
    }

    printf("%s", "Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    char attribute[15];
    scanf("%s", attribute);

    if (strcmp(attribute, "artist") == 0) { //strcmp compares 2 strings
        char artist[32];
        printf("%s", "Enter an artist: ");
        scanf(" %[^\n]%*c", artist);
        strcpy(songList[id].artist, artist);
    }

    if (strcmp(attribute, "title") == 0) {
        char title[32];
        printf("%s", "Enter a title: ");
        scanf(" %[^\n]%*c", title);
        strcpy(songList[id].title, title);

    }

    if (strcmp(attribute, "duration") == 0) {
        int durationMin;
        int durationSec;
        printf("%s", "Enter a duration (minutes): ");
        scanf("%d", &durationMin);
        songList[id].durationMin = durationMin;
        printf("%s", "Enter a duration (seconds): ");
        scanf("%d", &durationSec);
        songList[id].durationSec = durationSec;
    }

    if (strcmp(attribute, "danceability") == 0) {
        float dance;
        printf("%s", "Enter danceability: ");
        scanf("%f", &dance);
        songList[id].danceability = dance;
    }

    toString(songList);

    return 0;
}
