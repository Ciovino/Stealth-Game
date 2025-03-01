#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include "../header/face_color.h"

typedef struct Player* PLAYER;

// Memory managment
PLAYER NewPlayer(char face, int startX, int startY, int maxHeight, int maxWidth);
void FreePlayer(PLAYER p);

// Movement
int CalculatePosition(PLAYER p, int move);
void ConfirmePosition(PLAYER p);
void RejectPosition(PLAYER p);

// Update
int UpdatePlayer(PLAYER p);
void ResetPlayer(PLAYER p);

// Get player info
int GetPlayerPosition(PLAYER p);
FACE_COLOR GetPlayerFaceColor(PLAYER p);
char GetPlayerFace(PLAYER p);

// Get noise player info
int GetNoiseRadius(PLAYER p);
FACE_COLOR GetNoise(PLAYER p);
char GetNoiseFace(PLAYER p);

#endif