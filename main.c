#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

typedef struct {
    char identifier;
    int16_t offset;
} movable_t;

struct settings {
    movable_t* movables;
    char* name;
    uint8_t colorChange;
};

void printHelp(void);
void handleInput(int argc, char *argv[]);

int main(int argc, char *argv[]) {
    int16_t moveY = 0;
    char* cmpName = NULL;

    // Check for correct number of arguments
    if(argc < 4) {
        printf("Usage: %s <filename> -m<identifier character> <offset> -n <name of the object to be moved>\n", argv[0]);
        printf("\tExample: '%s example.gcode -mX 50 -n words' \n");
        printf("\tMoves object 'words' in the file 'example.gcode' 50mm in the X direction.");
        return 1;
    }

    
    // Open the file
    FILE *fp = fopen(argv[1], "r+");
    if(fp == NULL) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    // Close the file
    fclose(fp);
    return 0;
}


void printHelp(void) {

}