#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../header/face_color.h"

// Key for player movment
typedef enum{
    NO_INPUT = -1,
    UP = 119,       // W
    DOWN = 115,     // S
    RIGHT = 100,    // A
    LEFT = 97,      // D
    ESC = 27,       // esc (exit game)
}PLAYER_MOVEMENT;

typedef struct Player* PLAYER;

// Memory managment
PLAYER NewPlayer(char face, int startX, int startY, int maxHeight, int maxWidth);
void FreePlayer(PLAYER p);

// Movement
int MovePlayer(PLAYER p, PLAYER_MOVEMENT move);

// Update
int UpdatePlayer(PLAYER p);
void ResetPlayer(PLAYER p);

// Getters
int GetPlayerPosition(PLAYER p);
int GetNoiseRadius(PLAYER p);
FACE_COLOR GetPlayerFaceColor(PLAYER p);
char GetPlayerFace(PLAYER p);
FACE_COLOR GetNoise(PLAYER p);
char GetNoiseFace(PLAYER p);

#endif