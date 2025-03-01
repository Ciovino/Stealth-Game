#include <conio.h> // kbhit()

#include "../header/Screen.h"
#include "../header/map.h"

#define FPS 3

// Data found in the PowerShell settings
#define MAX_W 118   // 120 - 2
#define MAX_H 26    // 30 - 4

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
// #define POS(x, y, max_w) (x*max_w + y)

/*** Stealth Game ***/
int main(int argc, char **argv)
{
    ClearAndHome();

    // Map Initialization
    int width = 100, height = 100;
    MAP map = NewMap(MIN(width, MAX_W), MIN(height, MAX_H), FPS);

    // Add some guards
    CreateRandomGuards(map, 4);

    // Add player
    AddPlayer(map, 'S', 1, 1);

    StartGame(map);

    int keepGoing = 1;
    while(keepGoing)
    {
        int key = NO_INPUT;
        if(kbhit()) key = getch();

        keepGoing = MovePlayer(map, key);

        UpdateMap(map);
    }

    FreeMap(map);

    ClearAndHome();
    return 0;
}