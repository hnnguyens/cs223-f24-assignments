/*----------------------------------------------
 * Author: Hazel Nguyen 
 * Date: 09/20/2024
 * Description: Reads songs from a text file into an array.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main() {
    FILE *inFile; //cannot be dereferenced!! 

    printf("%s", "Welcome to Hazel's Danceability Directory!\n");

    inFile = fopen("songlist.csv", "r"); //read mode only!
    if (inFile == NULL) {
        printf("%s", "Error: Invalid file!\n");
        return 1; // meaning that the code terminates w/ an error
    }

    char line[129]; //buffer
    fgets(line, sizeof(line), inFile); //first line
    char* token = strtok(line, ",");
    int len = atoi(token);
    song *songList = (song*) malloc(sizeof(song) * len); 

    if (songList == NULL) { //checks if valid
        printf("Space unable to be allocated."); 
        return 1;
    }
    fgets(line, sizeof(line), inFile); //skips header

    int i = 0; //for idx in songList
    //reading and parsing input
    while (i != len) {
        int pos = 0;
        if (fgets(line, sizeof(line), inFile) != NULL) { //should be at line 3
            token = strtok(line, ",");
            while (token != 0) {
                if (pos == 0) { //put token into struct 
                    strcpy(songList[i].title, token);
                }

                if (pos == 1) { //artist
                    strcpy(songList[i].artist, token);
                }

                if (pos == 2) { //duration 
                    int duration = atoi(token);
                    songList[i].durationMin = (duration/1000)/60;
                    songList[i].durationSec = (duration/1000)%60;
                }

                if (pos == 3) { //danceability
                    songList[i].danceability = atof(token);
                }

                if (pos == 4) { //energy
                    songList[i].energy = atof(token);
                }

                if (pos == 5) { //tempo
                    songList[i].tempo = atof(token);
                }

                if (pos == 6) { //valence
                    songList[i].valence = atof(token);
                }
                token = strtok(0, ","); //advances in string
                pos++;
            }
            i++; //next song in songList
        }
    } 

    //printing to an attractive table
    for (int i = 0; i < len; i++) {
        if (i >= 10) {
            printf("%d) %-30s artist: %-20s duration: ",
                    i, songList[i].title,
                    songList[i].artist);
        }

        else {
            printf("%d)  %-30s artist: %-20s duration: ",
                    i, songList[i].title,
                    songList[i].artist);
        }

        if (songList[i].durationSec < 10) { //seconds < 10, needs 0
            printf("%d:0%d D: %.3f E: %.3f T: %.3f V: %.3f\n",
                    songList[i].durationMin,
                    songList[i].durationSec,
                    songList[i].danceability,
                    songList[i].energy,
                    songList[i].tempo,
                    songList[i].valence);
        }      

        else { //double digit
            printf("%d:%d D: %.3f E: %.3f T: %.3f V: %.3f\n",
                    songList[i].durationMin,
                    songList[i].durationSec,
                    songList[i].danceability,
                    songList[i].energy,
                    songList[i].tempo,
                    songList[i].valence);

        }      
    }

    printf("************************************************\n");
    free(songList); //always free after malloc!
    fclose(inFile); //closes the file
    return 0;
}
