#ifndef GUARD_H_INCLUDED
#define GUARD_H_INCLUDED

#include "..\header\face_color.h"

typedef enum{
    NORD    = -2, 
    EAST    = 1, 
    SOUTH   = 2,
    WEST    = -1
}GUARD_DIRECTION;

typedef struct guard *GUARD;

// Memory managment
GUARD NewGuard(int startX, int startY, int range, GUARD_DIRECTION direction, int speed, int maxHeight, int maxWidth);
void FreeGuard(GUARD g);

// Movment
int UpdateGuard(GUARD g);

// Getters
int GetGuardPosition(GUARD g);
int* GetRangePosition(GUARD g);
char GetGuardFace(GUARD g);
FACE_COLOR GetGuardFaceColor(GUARD g);
FACE_COLOR GetRangeCol(GUARD g);

#endif