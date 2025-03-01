#include <stdio.h>
#include <conio.h> // kbhit()

#include "..\header\Screen.h"

#include "../header/map.h"
#include "../header/player.h"

#define FPS 3

// Data found in the PowerShell settings
#define MAX_W 118   // 120 - 2
#define MAX_H 28    // 30 - 2

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
// #define POS(x, y, max_w) (x*max_w + y)

/*** Stealth Game ***/
int main(int argc, char **argv)
{
    ClearAndHome();

    // Map Initialization
    int width = 100, height = 20;
    MAP map = NewMap(MIN(width, MAX_W), MIN(height, MAX_H), FPS);

    // Add some guards
    CreateRandomGuards(map, 4);

    // Create a player
    PLAYER player = NewPlayer('S', 1, 1, GetMapHeight(map), GetMapWidth(map));
    AddPlayer(map, player);

    printf("this u -> %c\n", GetPlayerFace(player));
    while (!kbhit()) {}    

    // Start game
    PrintMap(map);

    FreeMap(map);
    return 0;
}