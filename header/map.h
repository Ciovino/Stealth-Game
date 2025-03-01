#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

#include "../header/player.h"
#include "../header/guard.h"
#include "..\header\face_color.h"

typedef struct map* MAP;

// Memory managment
MAP NewMap(int width, int height, int targetFps);
void FreeMap(MAP m);

// Player
void AddPlayer(MAP m, PLAYER p);

// Guards
void CreateRandomGuards(MAP m, int totalGuards);

// Map operations
void PrintMap(MAP m);
int GetMapWidth(MAP m);
int GetMapHeight(MAP m);

#endif