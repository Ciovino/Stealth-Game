#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // kbhit()
#include <time.h> // clock()

#include "..\header\Screen.h"
#include "..\header\Random.h"

#define FPS 6

// Data found in the PowerShell settings
#define MAX_W 120
#define MAX_H 30

// Key for player movment
#define NO_INPUT -1
#define UP      119     // W
#define DOWN    115     // S
#define RIGHT   100     // A
#define LEFT    97      // D
#define ESC     27      // ESC

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define POS(x, y) (x + y*MAX_W)

// Oposite direction have opposite sign
typedef enum{
    NORD    = -1, 
    EAST    = -2, 
    SOUTH   = 1,
    WEST    = 2
}DIRECTIONS;

/*** Stealth Game ***/
typedef struct{
    char face;
    int x;
    int y;
    int old_x;
    int old_y;
    int updated;
}PLAYER;

typedef struct{
    char face;
    int x;
    int y;
    int old_x;
    int old_y;

    DIRECTIONS direction;

    // How many frame it needs to move
    int speed;
    int frameCounter;
}GUARD;

typedef struct{
    int fullSize;
    char* map;
    PLAYER* player;
    
    int deltaTime;
    clock_t thisFrame;

    GUARD** allGuards;
    int totalGuards;
}MAP;

MAP* InitializeMap(int width, int heigth);
void AddPlayer(MAP* map, PLAYER* player);
void CreateRandomGuards(MAP* map, int totalGuards);
void UpdateMap(MAP* map);
int UpdateGuards(MAP* map);
void PrintMap(MAP* map);
void FreeMap(MAP* map);

void UpdatePosition(PLAYER* p, int key_pressed);

int main(int argc, char **argv)
{
    // Map Initialization
    MAP* map = InitializeMap(MAX_W, MAX_H);

    ClearAndHome();

    // Create a player
    printf("Choose your character (*smash bros theme in the background*): ");
    PLAYER* player = malloc(sizeof(PLAYER));
    player->x = player->y = player->old_x = player->old_y = player->updated = 0;
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
            UpdatePosition(player, key);
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
        map->map[POS(map->player->old_x, map->player->old_y)] = ' ';
        map->map[POS(map->player->x, map->player->y)] = map->player->face;
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
                    map->allGuards[g]->y--;
                    if(map->allGuards[g]->y < 0){
                        // Invert direction
                        map->allGuards[g]->direction *= -1;
                        map->allGuards[g]->y = map->allGuards[g]->old_y + 1;
                    }
                    break;

                case SOUTH:
                    map->allGuards[g]->y++;
                    if(map->allGuards[g]->y >= MAX_H){
                        // Invert direction
                        map->allGuards[g]->direction *= -1;
                        map->allGuards[g]->y = map->allGuards[g]->old_y - 1;
                    }
                    break;

                case EAST:
                    map->allGuards[g]->x++;
                    if(map->allGuards[g]->x >= MAX_W){
                        // Invert direction
                        map->allGuards[g]->direction *= -1;
                        map->allGuards[g]->x = map->allGuards[g]->old_x - 1;
                    }
                    break;

                case WEST:
                    map->allGuards[g]->x--;
                    if(map->allGuards[g]->x < 0){
                        // Invert direction
                        map->allGuards[g]->direction *= -1;
                        map->allGuards[g]->x = map->allGuards[g]->old_x + 1;
                    }
                    break;

                default: break;
            }

            map->map[POS(map->allGuards[g]->old_x, map->allGuards[g]->old_y)] = ' ';
            map->map[POS(map->allGuards[g]->x, map->allGuards[g]->y)] = map->allGuards[g]->face;
            map->allGuards[g]->frameCounter = 0;
        }
    }

    return update;
}

void UpdatePosition(PLAYER* p, int key_pressed)
{
    // Position already updated for this frame
    if(p->updated)
        return;

    p->old_x = p->x;
    p->old_y = p->y;

    p->updated = 1;
    switch (key_pressed) {
        case UP:
            p->y--;
            if(p->y < 0){
                p->y = 0;
                p->updated = 0;
            }
            break;

        case DOWN:
            p->y++;
            if(p->y >= MAX_H){
                p->y = MAX_H - 1;
                p->updated = 0;
            }
            break;

        case RIGHT:
            p->x++;
            if(p->x >= MAX_W){
                p->x = MAX_W - 1;
                p->updated = 0;
            }
            break;

        case LEFT:
            p->x--;
            if(p->x < 0){
                p->x = 0;
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
    printf("%s", map->map);
}

void AddPlayer(MAP* map, PLAYER* player)
{
    map->player = player;

    map->map[POS(map->player->x, map->player->y)] = map->player->face;
}

GUARD* RandomGuard()
{
    GUARD* g = malloc(sizeof(GUARD));

    g->old_x = g->x = RandomIntFrom0ToMax(MAX_W);
    g->old_y = g->y = RandomIntFrom0ToMax(MAX_H);
    g->direction = RandomInt(1, 3);

    g->speed = RandomInt(10, 15);
    g->face = 'G';
    g->frameCounter = 0;

    return g;
}

void CreateRandomGuards(MAP* map, int totalGuards)
{
    map->totalGuards = totalGuards;
    map->allGuards = malloc(totalGuards*sizeof(*map->allGuards));

    for(int g = 0; g < totalGuards; g++)
    {
        GUARD* guard = RandomGuard();

        // Add the new guard
        map->allGuards[g] = guard;

        map->map[POS(guard->x, guard->y)] = guard->face;
    }
}

MAP* InitializeMap(int width, int heigth)
{
    MAP* map = malloc(sizeof(MAP));

    map->fullSize = width*heigth;

    map->map = malloc((map->fullSize + 1) * sizeof(char));
    for(int i = 0; i < map->fullSize; i++) map->map[i] = ' '; 
    map->map[map->fullSize] = '\0';

    // No player are provided
    map->player = NULL;

    // Delta time => how much time between frame, in milliseconds
    map->deltaTime = 1000 / FPS;

    return map;
}

void FreeMap(MAP* map)
{
    for(int i = 0; i < map->totalGuards; i++)
        free(map->allGuards[i]);
    free(map->allGuards);

    free(map->map);
    free(map);
}
