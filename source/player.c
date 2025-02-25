#include "..\header\player.h"
#include "..\header\Colori.h"
#include <stdlib.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))

struct Player{
    // Position
    int x;
    int y;
    int old_x;
    int old_y;

    // Frame update
    int updated;
    
    // Character
    char face;
    COLOR faceColor;
    COLOR backGround;    

    // Noise radius
    int noise_radius;
    char noise_face;
    COLOR noiseColor;
    COLOR noiseBackGround;

    // Map Limit
    int maxHeigth;
    int maxWidth;
};

// New player
PLAYER NewPlayer(char face, int startX, int startY, int maxHeigth, int maxWidth)
{
    if(face == 0) face = 'S'; // Default character

    PLAYER p = malloc(sizeof(*p));

    // Starting position
    p->x = p->old_x = startX;
    p->y = p->old_y = startY;

    // Apperance
    p->face = face;
    p->noise_face = '~';
    p->faceColor = COL_LIGHT_CYAN;
    p->backGround = COL_BLACK;
    p->noiseColor = COL_LIGHT_BLUE;
    p->backGround = COL_BLACK;

    // Starting values
    p->updated = p->noise_radius = 0;

    // Map limit
    p->maxHeigth = maxHeigth;
    p->maxWidth = maxWidth;

    return p;
}

// Delete from memory
void FreePlayer(PLAYER p)
{
    free(p);
}

void MovePlayer(PLAYER p, PLAYER_MOVEMENT move)
{
    // Position already updated for this frame
    if(p->updated) return;

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
            if(p->x >= p->maxHeigth){
                p->x = p->maxHeigth - 1;
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
}

int UpdatePlayer(PLAYER p)
{
    if(p->updated)
        p->noise_radius++;
    else
        p->noise_radius = MAX(-1, p->noise_radius - 1);

    return p->updated || (p->noise_radius != -1);
}

void ResetPlayer(PLAYER p)
{
    p->updated = 0;
}