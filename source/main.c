#include "../header/map.h"
#include <conio.h> // kbhit()

#define FPS 3

// Data found in the PowerShell settings
#define MAX_W 118   // 120 - 2 (for map border)
#define MAX_H 26    // 30 - 4 (for map border + title)

#define MIN(x, y) (((x) < (y)) ? (x) : (y))

/*** Stealth Game ***/
int main(int argc, char **argv)
{
    // Map Initialization
    int width = 64, height = 18;
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
    return 0;
}