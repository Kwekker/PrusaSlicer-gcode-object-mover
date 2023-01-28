#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#include "objectMove.h"

#define GCODE_LINE_BUFFER_SIZE 256

static int16_t findNextObject(FILE* inFile, FILE* outFile, char* names[], uint16_t objectCount);
static uint8_t moveObject(FILE* inFile, FILE* outFile, objectSettings_t object);
static char* moveLine(char buffer[GCODE_LINE_BUFFER_SIZE], objectSettings_t object);
static int8_t findAxisIndex(objectSettings_t object, char c);

uint8_t moveObjects(char* inFileName, char* outFileName, objectSettings_t* objects, uint16_t objectCount) {

    FILE* inFile = fopen(inFileName, "r+");
    FILE* outFile;
    if (outFileName != NULL) {
        outFile = fopen(outFileName, "w");
        if (outFile == NULL) {
            printf("Couldn't create the specified output file :(\n");
            return -1;
        }
        printf("Writing to file %s\n", outFileName);
    }
    else outFile = tmpfile();

    char* names[objectCount];

    // Make an array of the names of the objects to make it easier to compare them
    // to the objects found in the file
    for (uint16_t i = 0; i < objectCount; i++) {
        names[i] = objects[i].name;
    }

    // Go through all specified objects and find them in the file.
    // Then apply the needed changes for each object in order of appearance in the file.
    for (uint16_t i = 0; i < objectCount; i++) {
        int16_t foundObjectIndex = findNextObject(inFile, outFile, names, objectCount);
        if (foundObjectIndex < 0) {
            fclose(inFile);
            fclose(outFile);
            return 1;
        }
        printf("Moving object %s\n", objects[foundObjectIndex].name);
        moveObject(inFile, outFile, objects[foundObjectIndex]);
    }

    if (outFileName == NULL) {
        printf("Overwriting input file %s\n", inFileName);
        fseek(inFile, 0, SEEK_SET);
        fseek(outFile, 0, SEEK_SET);
        // A nice oneliner to copy a file
        while (fputc(fgetc(outFile), inFile) != 0);
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
        // do-while because the buffer is uninitialized
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
                printf("Found object %s\n", names[i]);
                names[i] = NULL;
                return 0;
            }
        }
    }

}


// Goes through the inFile starting from the current file pointer
// and moves all required axes in each G1 command accordingly.
// Adds M600 (filament change) if needed.
// Stops when it finds the next object.
static uint8_t moveObject(FILE* inFile, FILE* outFile, objectSettings_t object) {

    // A color list for the different colors objects can be
    // This is literally just for the preview in the PrusaSlicer gcode viewer, it has no other use.
    static const char colors[7][7] = {"0000FF", "00FF00", "00FFFF", "FF0000", "FF00FF", "FFFF00", "FFFFFF"};
    
    if (!object.noColorChange) {
        char* colorChangeString = ";COLOR_CHANGE,T0,#XXXXXX\nM600\n";
        static uint8_t currentColor = 0;
        memcpy(colorChangeString + 18, colors[currentColor++], 6);
        if(currentColor > 7) currentColor = 0;

        fputs(colorChangeString, outFile);
    }
    

    char buffer[GCODE_LINE_BUFFER_SIZE];
    while (fgets(buffer, GCODE_LINE_BUFFER_SIZE, inFile) != NULL) {
        // Check if it's the beginning of the next object.
        if (!strncmp(buffer, "; printing object ", 18) && strncmp(buffer + 18, object.name, strlen(object.name))) {
            printf("End of object %s\n", object.name);
            return 0;
        }

        // Using a function is dumb when you're only checking 2 chars.
        // Checking for both G1 and G0 even though I'm pretty sure PrusaSlicer doesn't use G0.
        if (buffer[0] == 'G' && (buffer[1] == '1' || buffer[1] == '0')) { 
            char* putInFile = moveLine(buffer, object);
            // Make the changes
            fputs(putInFile, outFile);
        }
        else fputs(buffer, outFile);
    }

    return 0;
}


// Moves a single G1 line according to the objectSettings object.
// Returns the string with the new coordinates (as a pointer to a static string).
static char* moveLine(char inBuffer[GCODE_LINE_BUFFER_SIZE], objectSettings_t object) {
    static char outBuffer[GCODE_LINE_BUFFER_SIZE];
    strncpy(outBuffer, inBuffer, 3);
    char* inPtr = inBuffer + 3;
    char* outPtr = outBuffer + 3;

    for (uint8_t i = 0; i < object.axisCount; i++) {
        int8_t axisIndex;

        // Copy the line until an axis is found that has to be moved.
        // Using short circuiting because I'm a nerd who likes to optimize things.
        while (!isalpha(*inPtr) || -1 == (axisIndex = findAxisIndex(object, *inPtr))) {
            *(outPtr++) = *(inPtr++);
        }

        // One more time to get the identifier out of the way
        *(outPtr++) = *(inPtr++);

        // If the character after the initial axis identifying character isn't a number,
        // you're probably looking at a comment instead of an axis.
        // We don't want to change any comments, and comments are at the end of the line, so we break.
        if (!isnumber(*inPtr)) break;

        // Idk how big your printer is, maybe it is 2.15Mm long who knows
        int32_t newNum = atoi(inPtr) + object.axes[axisIndex].offset;
        outPtr += sprintf(outPtr, "%d", newNum); 

        // Make sure to skip the number in the input (before the decimal point)
        while (isnumber(*inPtr)) inPtr++;
    }

    //Copy the remaining part of the line
    strcpy(outPtr, inPtr);

    return outBuffer;
}


// Returns the index of the axis with the specified identifying char
// Or 0xff if it doesn't have that axis.
static int8_t findAxisIndex(objectSettings_t object, char c) {
    for (uint8_t i = 0; i < object.axisCount; i++) {
        if (object.axes[i].identifier == c) return i;
    }
    return -1;
}