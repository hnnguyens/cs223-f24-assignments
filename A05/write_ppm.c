/*----------------------------------------------
 * Author: Hazel Nguyen
 * Date: October 3rd, 2024
 * Description: Writes a ppm file. 
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

//Implement *one* (but do not remove the other one)!

/**
 * Writes a ppm file from a flat array. 
 * @param filename the file to copy from
 * @param pixels struct of pixels_ppm
 * @param w width 
 * @param h height 
 * @return void
*/
void write_ppm(const char* filename, struct ppm_pixel* pixels, int w, int h) { 
    FILE *outfile; 
    outfile = fopen(filename, "w"); //writing into the new file

    if (!outfile) {
        printf("Error: Unable to open file! %s\n", filename);
        return;
    }

    fprintf(outfile, "P6.\n #Created by Hazel Nguyen ver\n%d %d", w, h); //for header
    //put binary data into a new file; writing it
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            unsigned char pixelBlock[3] = {pixels[i * h + j].red, 
                pixels[i * h + j].green, pixels[i * h + j].blue};
            fwrite(pixelBlock, sizeof(unsigned char), 3, outfile); //copy
        }
    }
    fclose(outfile); //closes file & prevents memory leak!
}

/**
 * Writes a ppm file, reading from a 2D array. 
 * @param filename the ppm file to copy
 * @param pixels the 2D array struct 
 * @param w width 
 * @param h height 
 * @return void 
*/
void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) { 

}
