#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "objectMove.h"

uint8_t handleInput(int argc, char* argv[], objectSettings_t** objects, char** outFileName);
void printHelp(void);
void printObjects(objectSettings_t* objects, uint16_t objectCount);

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    // Check for correct number of arguments
    if(argc < 4) {
        printHelp();
    }

    char* outFileName = NULL;
    objectSettings_t* objects = 0;
    uint16_t objectCount = handleInput(argc, argv, &objects, &outFileName);
    if(objectCount == 0) return 1;
    
    if(moveObjects(argv[1], outFileName, objects, objectCount) == 0)
        printf("Done :)\n");

    return 0;
}


uint8_t handleInput(int argc, char* argv[], objectSettings_t** objects, char** outFileName) {

    // Count amount of objects and allocate the array
    uint16_t objectCount = 0;
    for(uint16_t i = 1; i < argc; i++) {
        if(argv[i][0] == '-' && argv[i][1] == 'N')
            objectCount++;
    }
    *objects = (objectSettings_t*) calloc(objectCount, sizeof(objectSettings_t));

    // Go through the arguments and save them
    objectSettings_t* currentObject = *objects - 1;
    // -1 because the N option goes to the next object before doing anything.
    // Not the prettiest but it works
    for(uint16_t i = 2; i < argc; i++) {

        if(argv[i][0] == '-') {
            switch(argv[i][1]) {
                case 'N':
                    if(i + 1 >= argc) {
                        printf("Please specify the object's name when using -N\n");
                        return 0;
                    }

                    currentObject++;

                    char* storeName = (char*)malloc(2 + strlen(argv[i + 1]));
                    sprintf(storeName, "%s ", argv[i + 1]);
                    currentObject->name = storeName;
                    
                    i++;
                    break;
                
                case 'a':
                    if(!isalpha(argv[i][2])) {
                        printf("Please specify an axis when using -a\n");
                        return 0;
                    }
                    if(i + 1 >= argc) {
                        printf("Please specify the offset amount when using -a\n");
                        return 0;
                    }

                    currentObject->axes[currentObject->axisCount].identifier = argv[i][2];
                    currentObject->axes[currentObject->axisCount].offset = atoi(argv[i + 1]);
                    currentObject->axisCount++;
                    i++;
                    break;

                case 'c':
                    currentObject->noColorChange = 1;
                    break;

                case 'O':
                    *outFileName = argv[i + 1];
                    i++;
                    break;
                
                case 'H':
                case 'h':
                    printHelp();
                    break;
                
                default:
                    printf("%s is not a valid option.\n", argv[i]);
                    return 0;
            }
        }
        else {
            printf("%s is not a valid option.\n", argv[i]);

            printHelp();
            return 0;
        }
    }

    if(objectCount == 0) printf("No objects were specified.\n");
    return objectCount;
}

void printHelp(void) {
    printf("\nUsage:\n");
    printf("The first argument should be the input file name. A better more detailed explanation can be found on git:\n");
    printf("https://github.com/kwekker/gcode-prusaslicer-object-mover\n");
    printf("-H\t\tShow help\n");
    printf("-O <filename>\tWrite an output file with name <filename> instead of overwriting the input file.\n");
    printf("-N <name>\tAll consecutive lowercase flags will be about the object named <name>.\n");
    printf("-a<I> <offset>\tSet the offset of an axis. <I> is the axis name (like 'X'), <offset> is an offset integer.\n");
    printf("-c\t\tNo filament change will be inserted before the object (so just don't use this flag if you want a filament change).\n");
    printf("\n");
}


void printObjects(objectSettings_t* objects, uint16_t objectCount) {
    for(uint8_t i = 0; i < objectCount; i++) {
        objectSettings_t* o = objects + i;
        printf("name: %s\nno color c: %d\naxis count: %d\n", o->name, o->noColorChange, o->axisCount);
        for(uint8_t j = 0; j < o->axisCount; j++) {
            printf("\tchar: %c\n\toffset: %d\n", o->axes[j].identifier, o->axes[j].offset);
        }
        printf("\n");
    }
}