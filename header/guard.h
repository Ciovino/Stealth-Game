#ifndef GUARD_H_INCLUDED
#define GUARD_H_INCLUDED

typedef enum{
    NORD    = -2, 
    EAST    = 1, 
    SOUTH   = 2,
    WEST    = -1
}GUARD_DIRECTION;

typedef struct guard *GUARD;

GUARD NewGuard(int startX, int startY, int range, GUARD_DIRECTION direction, int speed, int maxHeigth, int maxWidth);
int UpdateGuard(GUARD g);

#endif