#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "objectMove.h"

uint8_t handleInput(int argc, char* argv[], objectSettings_t** objects);
void printHelp(void);
void printObjects(objectSettings_t* objects, uint16_t objectCount);

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);

    // Check for correct number of arguments
    if(argc < 4) {
        printHelp();
    }

    objectSettings_t* objects = 0;
    uint16_t objectCount = handleInput(argc, argv, &objects);
    if(objectCount == 0) return 1;
    
    moveObjects(argv[1], objects, objectCount);

    return 0;
}

uint8_t handleInput(int argc, char* argv[], objectSettings_t** objects) {

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
    printf("TODO: Write the help thing hehe\n");
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