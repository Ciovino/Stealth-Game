#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // kbhit()
#include <time.h> // clock()

#include "..\header\Screen.h"
#include "..\header\Random.h"
#include "..\header\Colori.h"

#define FPS 6

// Data found in the PowerShell settings
#define MAX_W 38   // 120/3 - 2
#define MAX_H 28    // 30 - 2

// Key for player movment
#define NO_INPUT -1
#define UP      119     // W
#define DOWN    115     // S
#define RIGHT   100     // A
#define LEFT    97      // D
#define ESC     27      // ESC

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define POS(x, y, max_w) (x*max_w + y)

// Oposite direction have opposite sign
typedef enum{
    NORD    = -1, 
    EAST    = -2, 
    SOUTH   = 1,
    WEST    = 2
}DIRECTIONS;

typedef enum{
    C_NOTHING = -1,
    C_GUARD,
    C_GUARD_RANGE,
    C_PLAYER,
}SPECIAL_CHARS;

/*** Stealth Game ***/
typedef struct{
    char face;
    int x;
    int y;
    int old_x;
    int old_y;
    int updated;
    
    COLOR faceColor;
    COLOR backGround;
}PLAYER;

typedef struct{
    char face;
    int x;
    int y;
    int old_x;
    int old_y;

    DIRECTIONS direction;
    int range;
    char rangeChar;
    int* rangePos;

    // How many frame it needs to move
    int speed;
    int frameCounter;
}GUARD;

typedef struct{
    int width, height;
    int fullSize;
    char* map;
    PLAYER* player;
    
    int deltaTime;
    clock_t thisFrame;

    GUARD** allGuards;
    int totalGuards;
    COLOR guardColor;
    COLOR guardBackGround;

    char* specialChar;
    int specialCharSize;
}MAP;

MAP* InitializeMap(int width, int height);
SPECIAL_CHARS FindSpecialChar(MAP* map, char c);
void AddPlayer(MAP* map, PLAYER* player);
void CreateRandomGuards(MAP* map, int totalGuards);
int* ComputeRangePosition(int* oldRange, int range, int x, int y, DIRECTIONS direction, int max_width);
void UpdateMap(MAP* map);
int UpdateGuards(MAP* map);
void PrintMap(MAP* map);
void FreeMap(MAP* map);

void UpdatePosition(MAP* map, PLAYER* p, int key_pressed);

int main(int argc, char **argv)
{
    ClearAndHome();

    // Map Initialization
    int map_width = 60, map_height = 20;
    MAP* map = InitializeMap(MIN(MAX_W, map_width), MIN(MAX_H, map_height));
    printf("Map size: %dx%d\n", map->width, map->height);

    // Create a player
    printf("Choose your character (*smash bros theme in the background*): ");
    PLAYER* player = malloc(sizeof(PLAYER));
    player->x = player->y = player->old_x = player->old_y = player->updated = 0;
    player->faceColor = COL_LIGHT_CYAN; player->backGround = COL_BLACK;
    player->face = getchar(); // Let player choose their own character

    printf("this u -> %c\n", player->face);
    while (!kbhit()) {}

    // Create some guards
    CreateRandomGuards(map, 10);

    // Start game
    map->thisFrame = clock();
    AddPlayer(map, player);
    PrintMap(map);

    int esc = 0;
    while(!esc)
    {
        int key = NO_INPUT;

        // Check for input
        if(kbhit()) {
            key = getch();
        }

        switch (key) {
        case ESC: esc = 1; break;

        case UP:
        case DOWN:
        case RIGHT:
        case LEFT:
            UpdatePosition(map, player, key);
            break;

        default:
            break;
        }

        UpdateMap(map);
    }
    
    FreeMap(map);
    free(player);
    return 0;
}

void UpdateMap(MAP* map)
{
    clock_t time = clock();
    if(time - map->thisFrame < map->deltaTime)
        return;

    // Player update
    if(map->player->updated)
    {
        map->map[POS(map->player->old_x, map->player->old_y, map->width)] = ' ';
        map->map[POS(map->player->x, map->player->y, map->width)] = map->player->face;
    }

    int guardUpdate = UpdateGuards(map);

    int someUpdate = map->player->updated || guardUpdate;
    if(someUpdate) PrintMap(map);

    map->thisFrame = clock();
    map->player->updated = 0;
}

int UpdateGuards(MAP* map)
{
    int update = 0;
    for(int g = 0; g < map->totalGuards; g++)
    {
        map->allGuards[g]->frameCounter++;

        if(map->allGuards[g]->frameCounter >= map->allGuards[g]->speed) // Move
        {
            update = 1;

            map->allGuards[g]->old_x = map->allGuards[g]->x;
            map->allGuards[g]->old_y = map->allGuards[g]->y;

            switch(map->allGuards[g]->direction){
                case NORD:
                    map->allGuards[g]->x--;
                    if(map->allGuards[g]->x < 0){
                        // Invert direction
                        map->allGuards[g]->direction *= -1;
                        map->allGuards[g]->x++;
                    }
                    break;

                case SOUTH:
                    map->allGuards[g]->x++;
                    if(map->allGuards[g]->x >= map->height){
                        // Invert direction
                        map->allGuards[g]->direction *= -1;
                        map->allGuards[g]->x--;
                    }
                    break;

                case EAST:
                    map->allGuards[g]->y++;
                    if(map->allGuards[g]->y >= map->width){
                        // Invert direction
                        map->allGuards[g]->direction *= -1;
                        map->allGuards[g]->y--;
                    }
                    break;

                case WEST:
                    map->allGuards[g]->y--;
                    if(map->allGuards[g]->y < 0){
                        // Invert direction
                        map->allGuards[g]->direction *= -1;
                        map->allGuards[g]->y++;
                    }
                    break;

                default: break;
            }

            map->allGuards[g]->rangePos = ComputeRangePosition(
                map->allGuards[g]->rangePos, 
                map->allGuards[g]->range, 
                map->allGuards[g]->x,
                map->allGuards[g]->y, 
                map->allGuards[g]->direction,
                map->width);

            map->map[POS(map->allGuards[g]->old_x, map->allGuards[g]->old_y, map->width)] = ' ';
            map->map[POS(map->allGuards[g]->x, map->allGuards[g]->y, map->width)] = map->allGuards[g]->face;
            map->allGuards[g]->frameCounter = 0;

            for(int i = 0; i < map->allGuards[g]->range; i++)
            {
                int pos = map->allGuards[g]->rangePos[i];
                if(pos >= 0 && pos < map->fullSize)
                    map->map[pos] = 'g';
            }
        }
    }

    return update;
}

void UpdatePosition(MAP* map, PLAYER* p, int key_pressed)
{
    // Position already updated for this frame
    if(p->updated)
        return;

    p->old_x = p->x;
    p->old_y = p->y;

    p->updated = 1;
    switch (key_pressed) {
        case UP:
            p->x--;
            if(p->x < 0){
                p->x = 0;
                p->updated = 0;
            }
            break;

        case DOWN:
            p->x++;
            if(p->x >= map->height){
                p->x = map->height - 1;
                p->updated = 0;
            }
            break;

        case RIGHT:
            p->y++;
            if(p->y >= map->width){
                p->y = map->width - 1;
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

void PrintMap(MAP* map)
{
    ClearAndHome();
    
    // Top border
    printf("/");
    for(int i = 0; i < 3*map->width; i++) printf("-");
    printf("\\\n");

    // Map
    for(int r = 0; r < map->height; r++)
    {
        printf("|");
        for(int c = 0; c < map->width; c++)
        {
            char toPrint = map->map[POS(r, c, map->width)];
            SPECIAL_CHARS isSpecial = FindSpecialChar(map, toPrint);

            switch(isSpecial)
            {
                case C_PLAYER:
                    BackGroundAndText(map->player->backGround, map->player->faceColor);
                    printf("-%c-", toPrint);
                    break;
                
                case C_GUARD:
                    BackGroundAndText(map->guardBackGround, map->guardColor);
                    printf("-%c-", toPrint);
                    break;

                case C_GUARD_RANGE:
                    BackGroundAndText(map->guardBackGround, map->guardColor);
                    printf("   ");
                    break;
                
                case C_NOTHING:
                default:
                    BackGroundAndText(COL_BLACK, COL_NORMAL);
                    printf(" %c ",toPrint);
                    break;
            }
            BackGroundAndText(COL_BLACK, COL_NORMAL);
        }
        printf("|\n");
    }

    // Bottom border
    printf("\\");
    for(int i = 0; i < 3*map->width; i++) printf("-");
    printf("/");;
}

void AddPlayer(MAP* map, PLAYER* player)
{
    map->player = player;

    map->map[POS(map->player->x, map->player->y, map->width)] = map->player->face;

    map->specialChar[C_PLAYER] = map->player->face;
}

int* ComputeRangePosition(int* oldRange, int range, int x, int y, DIRECTIONS direction, int max_width)
{
    int* rangePos = oldRange;
    if(rangePos == NULL) rangePos = malloc(range * sizeof(int));

    int pos = POS(x, y, max_width), move;
    switch(direction)
    {
        case NORD:
            move = -max_width;
            break;

        case SOUTH:
            move = max_width;
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

        if((direction == NORD || direction == SOUTH) ||
            ((direction == EAST || direction == WEST) && ((nextPosition / max_width) == (pos / max_width))))
            rangePos[i] = nextPosition;
        else{
            rangePos[i] = -1;
        } 
    }
        

    return rangePos;
}

GUARD* RandomGuard(int width, int height)
{
    GUARD* g = malloc(sizeof(GUARD));

    g->old_x = g->x = RandomIntFrom0ToMax(height);
    g->old_y = g->y = RandomIntFrom0ToMax(width);
    g->direction = RandomInt(1, 3);

    g->range = RandomInt(1, 5);;
    g->rangePos = ComputeRangePosition(NULL, g->range, g->x, g->y, g->direction, width);

    g->face = 'G';
    g->rangeChar = 'g';   

    g->speed = 10;
    g->frameCounter = 0;

    return g;
}

void CreateRandomGuards(MAP* map, int totalGuards)
{
    map->totalGuards = totalGuards;
    map->allGuards = malloc(totalGuards*sizeof(*map->allGuards));

    for(int g = 0; g < totalGuards; g++)
    {
        GUARD* guard = RandomGuard(map->width, map->height);

        // Add the new guard
        map->allGuards[g] = guard;
        map->map[POS(guard->x, guard->y, map->width)] = guard->face;

        // Add guard Range
        for(int i = 0; i < guard->range; i++)
        {
            int pos = guard->rangePos[i];
            if(pos >= 0 && pos < map->fullSize)
                map->map[pos] = 'g';
        }
    }

    map->specialChar[C_GUARD] = 'G';
    map->specialChar[C_GUARD_RANGE] = 'g';
    map->guardColor = COL_BLACK;
    map->guardBackGround = COL_LIGHT_BLUE;
}

SPECIAL_CHARS FindSpecialChar(MAP* map, char c)
{
    SPECIAL_CHARS found = C_NOTHING;

    for(int i = 0; i < map->specialCharSize; i++)
    {
        if(map->specialChar[i] == c){
            found = i;
            break;
        }
    }
    return found;
}

MAP* InitializeMap(int width, int height)
{
    MAP* map = malloc(sizeof(MAP));

    map->width = width;
    map->height = height;
    map->fullSize = width*height;

    map->map = malloc((map->fullSize + 1) * sizeof(char));
    for(int i = 0; i < map->fullSize; i++) map->map[i] = ' '; 
    map->map[map->fullSize] = '\0';

    // No player are provided
    map->player = NULL;

    // Delta time => how much time between frame, in milliseconds
    map->deltaTime = 1000 / FPS;

    // Special character, for colors
    map->specialCharSize = 15;
    map->specialChar = malloc(map->specialCharSize * sizeof(char));
    for(int i = 0; i < map->specialCharSize; i++) map->specialChar[i] = '.';

    return map;
}

void FreeMap(MAP* map)
{
    for(int i = 0; i < map->totalGuards; i++)
        free(map->allGuards[i]);
    free(map->allGuards);

    free(map->map);
    free(map->specialChar);
    free(map);
}
