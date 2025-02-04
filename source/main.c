#include <stdio.h>
#include <stdlib.h>
#include <conio.h> // kbhit()
#include "..\header\Screen.h"

// Data found in the PowerShell settings
#define MAX_W 120
#define MAX_H 30

// Key for player movment
#define UP      119     // W
#define DOWN    115     // S
#define RIGHT   100     // A
#define LEFT    97      // D
#define ESC     27      // ESC

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define POS(x, y) (x + y*MAX_W)

/*** Stealth Game ***/
typedef struct{
    char face;
    int x;
    int y;
    int old_x;
    int old_y;
}PLAYER;

char* map;

int UpdatePosition(PLAYER* p, int key_pressed);

int main(int argc, char **argv)
{
    // Map Initialization
    int fullSize = MAX_W*MAX_H;
    map = malloc((fullSize + 1)*sizeof(char));
    for(int i = 0; i < fullSize; i++)
        map[i] = ' ';
    map[fullSize] = '\0';

    ClearAndHome();

    printf("Choose your character (*smash bros theme in the background*): ");
    PLAYER* player = malloc(sizeof(PLAYER));
    player->x = player->y = player->old_x = player->old_y = 0;
    player->face = getchar(); // Let player choose their own character

    printf("this u -> %c\n", player->face);

    // Start game
    int esc = 0;
    map[POS(player->x, player->y)] = player->face;

    ClearAndHome();
    printf("%s", map);

    while(!esc)
    {
        int key, update_frame = 1, update_player = 1;
        while (!kbhit()) {}
        key = getch();

        switch (key) {
        case ESC: esc = 1; break;

        case UP:
        case DOWN:
        case RIGHT:
        case LEFT:
            update_player = UpdatePosition(player, key);
            break;

        default:
            break;
        }

        update_frame = update_player;
        if(update_frame)
        {
            if(update_player)
            {
                map[POS(player->old_x, player->old_y)] = ' ';
                map[POS(player->x, player->y)] = player->face;
            }

            ClearAndHome();
            printf("%s", map);
        }
    }
    
    
    free(map);
    free(player);
    return 0;
}

int UpdatePosition(PLAYER* p, int key_pressed)
{
    p->old_x = p->x;
    p->old_y = p->y;

    int update_player_position = 1;
    switch (key_pressed) {
        case UP:
            p->y--;
            if(p->y < 0){
                p->y = 0;
                update_player_position = 0;
            }
            break;

        case DOWN:
            p->y++;
            if(p->y >= MAX_H){
                p->y = MAX_H - 1;
                update_player_position = 0;
            }
            break;

        case RIGHT:
            p->x++;
            if(p->x >= MAX_W){
                p->x = MAX_W - 1;
                update_player_position = 0;
            }
            break;

        case LEFT:
            p->x--;
            if(p->x < 0){
                p->x = 0;
                update_player_position = 0;
            }
            break;

        default:
            break;
    }

    return update_player_position;
}