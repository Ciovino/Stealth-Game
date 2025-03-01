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

// New player
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

// Delete from memory
void FreePlayer(PLAYER p)
{
    FreeFaceColor(p->face);
    FreeFaceColor(p->noise);
    free(p);   
}

static int CheckMove(PLAYER_MOVEMENT move)
{
    int res = 0;
    switch (move) {
        case UP:
        case DOWN:
        case RIGHT:
        case LEFT:
            res =  1;
            break;
        
        case ESC: 
            res = -1;
            break;

        default:
            break;
    }

    return res;
}

int MovePlayer(PLAYER p, PLAYER_MOVEMENT move)
{
    // Position already updated for this frame
    if(p->updated) return 0;

    int check = CheckMove(move);
    if(check != 1) return check; 

    p->old_x = p->x;
    p->old_y = p->y;

    p->updated = 1;
    switch (move) {
        case UP:
            p->x--;
            if(p->x < 0){
                p->x = 0;
                p->updated = 0;
            }
            break;

        case DOWN:
            p->x++;
            if(p->x >= p->maxHeight){
                p->x = p->maxHeight - 1;
                p->updated = 0;
            }
            break;

        case RIGHT:
            p->y++;
            if(p->y >= p->maxWidth){
                p->y = p->maxWidth - 1;
                p->updated = 0;
            }
            break;

        case LEFT:
            p->y--;
            if(p->y < 0){
                p->y = 0;
                p->updated = 0;
            }
            break;

        default:
            break;
    }

    return 1;
}

int UpdatePlayer(PLAYER p)
{
    if(p->updated)
        p->noiseRadius++;
    else
        p->noiseRadius = MAX(-1, p->noiseRadius - 1);

    return p->updated || (p->noiseRadius != -1);
}

void ResetPlayer(PLAYER p)
{
    p->updated = 0;
}

int GetPlayerPosition(PLAYER p)
{
    return POS(p->x, p->y, p->maxWidth);
}

int GetNoiseRadius(PLAYER p)
{
    return p->noiseRadius;
}

FACE_COLOR GetPlayerFaceColor(PLAYER p)
{
    return p->face;
}

char GetPlayerFace(PLAYER p)
{
    return GetFace(p->face);
}

FACE_COLOR GetNoise(PLAYER p)
{
    return p->noise;
}

char GetNoiseFace(PLAYER p)
{
    return GetFace(p->noise);
}