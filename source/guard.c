#include "../header/guard.h"

#include <stdlib.h>

#define POS(x, y, width) (x*width + y)

struct guard{
    // Position
    int x;
    int y;
    int old_x;
    int old_y;

    // Character
    FACE_COLOR face;

    // Range
    FACE_COLOR range;
    int rangeLength;
    int* rangePos;

    // Movment
    GUARD_DIRECTION direction;
    int speed;
    int frameCounter;

    // Map Limit
    int maxHeight;
    int maxWidth;
};

static int* ComputeRange(int* oldRange, int range, int x, int y, GUARD_DIRECTION direction, int maxWidth, int maxHeight)
{
    int* newRange = oldRange;
    if(newRange == NULL) newRange = malloc(range * sizeof(*newRange));

    int pos = POS(x, y, maxWidth), move;
    switch(direction)
    {
        case NORD:
            move = -maxWidth;
            break;

        case SOUTH:
            move = maxWidth;
            break;
            
        case EAST:
            move = 1;
            break;
            
        case WEST:
            move = -1;
            break;
            
        default:
            move = 0;
            break;
    }

    for(int i = 0; i < range; i++)
    {
        int nextPosition = pos + move*(i + 1);

        if(((direction == NORD || direction == SOUTH) && (nextPosition >= 0 && nextPosition < maxHeight*maxWidth)) ||
            ((direction == EAST || direction == WEST) && ((nextPosition / maxWidth) == (pos / maxWidth))))
            newRange[i] = nextPosition;
        else{
            newRange[i] = -1;
        }
    }

    return newRange;
}

GUARD NewGuard(int startX, int startY, int range, GUARD_DIRECTION direction, int speed, int maxHeight, int maxWidth)
{
    GUARD g = malloc(sizeof(*g));

    // Starting position
    g->x = g->old_x = startX;
    g->y = g->old_y = startY;

    // Character
    g->face = NewFaceColor('G', COL_BLACK, COL_LIGHT_BLUE);

    // Range
    g->range = NewFaceColor('g', COL_LIGHT_BLUE, COL_LIGHT_BLUE);
    g->rangeLength = range;
    g->rangePos = ComputeRange(NULL, range, startX, startY, direction, maxWidth, maxHeight);

    // Movment
    g->direction = direction;
    g->speed = speed;
    g->frameCounter = 0;

    // Map limit
    g->maxHeight = maxHeight;
    g->maxWidth = maxWidth;

    return g;
}

void FreeGuard(GUARD g)
{
    FreeFaceColor(g->face);
    FreeFaceColor(g->range);
    free(g->rangePos);
    free(g);
}

int UpdateGuard(GUARD g)
{
    g->frameCounter++;

    if(g->frameCounter >= g->speed)
    {
        g->frameCounter = 0;

        g->old_x = g->x;
        g->old_y = g->y;

        switch(g->direction)
        {
            case NORD:
                g->x--;
                if(g->x < 0)
                {
                    // Invert direction
                    g->direction *= -1;
                    g->x++;
                }
                break;

            case SOUTH:
                g->x++;
                if(g->x >= g->maxHeight)
                {
                    // Invert direction
                    g->direction *= -1;
                    g->x--;
                }
                break;

            case EAST:
                g->y++;
                if(g->y >= g->maxWidth)
                {
                    // Invert direction
                    g->direction *= -1;
                    g->y--;
                }
                break;

            case WEST:
                g->y--;
                if(g->y < 0)
                {
                    // Invert direction
                    g->direction *= -1;
                    g->y++;
                }
                break;

            default: break;
        }

        g->rangePos = ComputeRange(g->rangePos, g->rangeLength, g->x, g->y, g->direction, g->maxWidth, g->maxHeight);
    }

    return g->frameCounter == 0;
}

int GetGuardPosition(GUARD g)
{
    return POS(g->x, g->y, g->maxWidth);
}

int* GetRangePositions(GUARD g)
{
    return g->rangePos;
}

int GetRange(GUARD g)
{
    return g->rangeLength;
}

char GetGuardFace(GUARD g)
{
    return GetFace(g->face);
}

FACE_COLOR GetGuardFaceColor(GUARD g)
{
    return g->face;
}

char GetRangeFace(GUARD g)
{
    return GetFace(g->range);
}

FACE_COLOR GetRangeCol(GUARD g)
{
    return g->range;
}