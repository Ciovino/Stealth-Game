#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "../header/player.h"
#include "../header/guard.h"
#include "../header/face_color.h"

typedef struct map* MAP;

// Key for player movment
typedef enum{
    NO_INPUT = -1,
    UP = 119,       // W
    DOWN = 115,     // S
    RIGHT = 100,    // A
    LEFT = 97,      // D
    ESC = 27,       // esc (exit game)
}PLAYER_MOVEMENT;


// Memory managment
MAP NewMap(int width, int height, int targetFps);
void FreeMap(MAP m);

// Player
void AddPlayer(MAP m, char face, int startX, int startY);
int MovePlayer(MAP m, PLAYER_MOVEMENT move);

// Guards
void CreateRandomGuards(MAP m, int totalGuards);

// Map operations
void PrintMap(MAP m);
int GetMapWidth(MAP m);
int GetMapHeight(MAP m);
void StartGame(MAP m);
void UpdateMap(MAP m);

#endif