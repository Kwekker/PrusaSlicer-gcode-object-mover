#ifndef _OBJECTMOVE_H_
#define _OBJECTMOVE_H_

#include <stdint.h>

#define MAX_AXES 6

typedef struct {
    char identifier;
    int16_t offset;
} axis_t;

typedef struct {
    axis_t axes[MAX_AXES];
    uint8_t axisCount;
    char* name;
    uint8_t noColorChange;
} objectSettings_t;

uint8_t moveObjects(char* fileName, objectSettings_t* objects, uint16_t objectCount);

//TODO: Voer de kat

#endif // _OBJECTMOVE_H_