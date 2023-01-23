#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "objectMove.h"

#define GCODE_LINE_BUFFER_SIZE 256

static int16_t findNextObject(FILE* inFile, FILE* outFile, char* names[], uint16_t objectCount);

uint8_t moveObjects(char* fileName, objectSettings_t* objects, uint16_t objectCount) {

    FILE* inFile = fopen(fileName, "r+");
    FILE* outFile = tmpfile();
    char* names[objectCount];

    // Make an array of the names of the objects to make it easier to compare them
    // to the objects found in the file
    for (uint16_t i = 0; i < objectCount; i++) {
        names[i] = objects[i].name;
    }

    for (uint16_t i = 0; i < objectCount; i++) {
        int16_t foundObjectIndex = findNextObject(inFile, outFile, names, objectCount);
        if (foundObjectIndex < 0) {
            fclose(inFile);
            fclose(outFile);
            return 1;
        }
    }
    
    
    fseek(outFile, -1000, SEEK_END);
    for (uint16_t i = 0; i < 200; i++) {
        char buffer[GCODE_LINE_BUFFER_SIZE];
        if (fgets(buffer, GCODE_LINE_BUFFER_SIZE, outFile) == NULL) break;
        printf("%s", buffer);
    }
    
    fclose(inFile);
    fclose(outFile);
    return 0; 
}


// Moves the filepointer to the first occurrence of any of the object names in names[].
// Then sets the pointer to that name in names[] to NULL.
// While doing this it copies the content of the input file into the output file. 
// Returns the index of the object or -1 if it failed to find any objects.
static int16_t findNextObject(FILE* inFile, FILE* outFile, char* names[], uint16_t objectCount) {
    
    char buffer[GCODE_LINE_BUFFER_SIZE];
    while (1) {
        do {
            if (fgets(buffer, GCODE_LINE_BUFFER_SIZE, inFile) == NULL) {
                printf("Error: Could not find all objects.\n");
                return -1;
            }
            fputs(buffer, outFile);
        }
        while (strncmp(buffer, "; printing object ", 18));

        char* objectInFileName = buffer + 18;
        for (uint16_t i = 0; i < objectCount; i++) {
            if (names[i] != NULL && strncmp(objectInFileName, names[i], strlen(names[i])) == 0) {
                printf("Found object %s.\n", names[i]);
                names[i] = NULL;
                return 0;
            }
        }
    }

}
