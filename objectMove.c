#include <stdio.h>
#include "objectMove.h"

uint8_t moveObjects(char* fileName, objectSettings_t* objects, uint16_t objectCount) {

    char outFileName[strlen(fileName) + 6];
    sprintf(outFileName, "%s1ym0UF", fileName);
    FILE* inFile = fopen(fileName, "r+");
    FILE* outFile = fopen(fileName, "w");

    //TODO: make the program

    return 0; 
}