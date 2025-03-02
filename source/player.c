#include "../header/player.h"

#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define POS(x, y, max_w) (x*max_w + y)

struct Player{
    // Position
    int x;
    int y;
    int old_x;
    int old_y;

    // Frame update
    int updated;
    
    // Character
    FACE_COLOR face;

    // Noise radius
    int noiseRadius;
    FACE_COLOR noise;

    // Map Limit
    int maxHeight;
    int maxWidth;
};

// Create a new player
PLAYER NewPlayer(char face, int startX, int startY, int maxHeight, int maxWidth)
{
    if(face == 0) face = 'S'; // Default character

    PLAYER p = malloc(sizeof(*p));

    // Starting position
    p->x = p->old_x = startX;
    p->y = p->old_y = startY;

    // Apperance
    p->face = NewFaceColor(face, COL_LIGHT_CYAN, COL_BLACK);
    p->noise = NewFaceColor('~', COL_LIGHT_CYAN, COL_BLACK);

    // Starting values
    p->updated = p->noiseRadius = 0;

    // Map limit
    p->maxHeight = maxHeight;
    p->maxWidth = maxWidth;

    return p;
}

// Delete player and free the memory
void FreePlayer(PLAYER p)
{
    FreeFaceColor(p->face);
    FreeFaceColor(p->noise);
    free(p);   
}

// Check for valid key
static int CheckMove(int move)
{
    int res = 0;
    switch (move) {
        case 119:
        case 115:
        case 100:
        case 97:
            res =  1;
            break;
        
        case 27: 
            res = -1;
            break;

        default:
            break;
    }

    return res;
}

// Calculate new player position
int CalculatePosition(PLAYER p, int move)
{
    // ESC pressed, exit from game
    if(CheckMove(move) == -1) return -2*p->maxWidth;

    // Save old position
    p->old_x = p->x;
    p->old_y = p->y;

    // Position already updated this frame
    if(p->updated) return POS(p->x, p->y, p->maxWidth);

    // Compute new position
    switch (move) {
        case 119:
            p->x--;
            break;

        case 115:
            p->x++;
            break;

        case 100:
            p->y++;
            break;

        case 97:
            p->y--;
            break;

        default:
            break;
    }

    return POS(p->x, p->y, p->maxWidth);
}

// The new position is valid, schedule the update for next frame
void ConfirmePosition(PLAYER p)
{
    p->updated = 1;
}

// The new position was illegal, return to old position
void RejectPosition(PLAYER p)
{   
    p->x = p->old_x;
    p->y = p->old_y;
}

// Update player and player noise
int UpdatePlayer(PLAYER p)
{
    if(p->updated)
        p->noiseRadius++;
    else
        p->noiseRadius = MAX(-1, p->noiseRadius - 1);

    return p->updated || (p->noiseRadius != -1);
}

// Reset internal update
void ResetPlayer(PLAYER p)
{
    p->updated = 0;
}

// Return player current position
int GetPlayerPosition(PLAYER p)
{
    return POS(p->x, p->y, p->maxWidth);
}

// Return player old position
int GetPlayerOldPosition(PLAYER p)
{
    return POS(p->old_x, p->old_y, p->maxWidth);
}

// Get player character
char GetPlayerFace(PLAYER p)
{
    return GetFace(p->face);
}

// Get player character full information
FACE_COLOR GetPlayerFaceColor(PLAYER p)
{
    return p->face;
}

// Return player noise radius
int GetNoiseRadius(PLAYER p)
{
    return p->noiseRadius;
}

// Get player noise character
char GetNoiseFace(PLAYER p)
{
    return GetFace(p->noise);
}

// Get player noise character full information
FACE_COLOR GetNoise(PLAYER p)
{
    return p->noise;
}