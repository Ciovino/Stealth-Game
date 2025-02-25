#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

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
PLAYER NewPlayer(char face, int startX, int startY, int maxHeigth, int maxWidth);
void FreePlayer(PLAYER p);

// Movement
void MovePlayer(PLAYER p, PLAYER_MOVEMENT move);

// Update
int UpdatePlayer(PLAYER p);
void ResetPlayer(PLAYER p);

#endif