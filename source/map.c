#include "../header/map.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "../header/Random.h"
#include "../header/Screen.h"

#define POS(x, y, max_w) (x*max_w + y)

struct map {
    // Dimension
    int width; 
    int height;
    int fullSize;

    // Map
    char* map;
    char* baseMap; // without player or guards

    // Player info
    PLAYER player;

    // Guards info
    GUARD* allGuards;
    int totalGuards;

    // Character info
    FC_ARRAY specialChar;

    // Frame info
    int deltaTime;
    clock_t thisFrame;
};

MAP NewMap(int width, int height, int targetFps)
{
    MAP m = malloc(sizeof(*m));

    // Dimension
    m->width = width;
    m->height = height;
    m->fullSize = width*height;

    // Map
    m->map = malloc((m->fullSize + 1) * sizeof(*m->map));
    m->baseMap = malloc((m->fullSize + 1) * sizeof(*m->baseMap));

    // Build empty map
    for(int i = 0; i < m->fullSize; i++) m->baseMap[i] = m->map[i] = ' ';
    m->baseMap[m->fullSize] = m->map[m->fullSize] = '\0';

    // Player info
    m->player = NULL;

    // Guards info
    m->allGuards = NULL;
    m->totalGuards = 0;

    // Character info
    m->specialChar = NewFCArray();
    FACE_COLOR empty = NewFaceColor(' ', COL_NORMAL, COL_BLACK);
    AddFC(m->specialChar, empty);

    // Frame info
    m->deltaTime = 1000 / targetFps; // milliseconds between frames
    m->thisFrame = 0;

    return m;
}

void FreeMap(MAP m)
{
    free(m->map);
    free(m->baseMap);
    FreePlayer(m->player);
    FreeFCArray(m->specialChar);

    for(int i = 0; i < m->totalGuards; i++) FreeGuard(m->allGuards[i]);
    free(m->allGuards);

    free(m);
}

static int SetPlayerOnMap(MAP m)
{
    int update = UpdatePlayer(m->player);

    int noiseRadius = GetNoiseRadius(m->player),
        playerPos = GetPlayerPosition(m->player);

    for(int i = -noiseRadius; i <= noiseRadius; i++)
    {
        int row = (playerPos + i*m->width)/m->width;

        // Entire row out of bounds
        if(row < 0 || row >= m->height) continue;

        for(int j = -noiseRadius; j <= noiseRadius; j++)
        {
            int noisePos = playerPos + i*m->width +j;
            
            // Out of bounds
            if(noisePos < 0 || noisePos >= m->fullSize) continue;
            // Not on the same row
            if(noisePos/m->width != row) continue;

            m->map[noisePos] = GetNoiseFace(m->player);
        }
    }

    m->map[playerPos] = GetPlayerFace(m->player);

    ResetPlayer(m->player);
    return update;
}

void AddPlayer(MAP m, char face, int startX, int startY)
{
    m->player = NewPlayer(face, startX, startY, m->height, m->width);

    // Save characteristics
    AddFC(m->specialChar, GetPlayerFaceColor(m->player));
    AddFC(m->specialChar, GetNoise(m->player));
}

static int EvaluatePlayerPosition(MAP m, int newPosition)
{
    // Check Out of Bounds
    if(newPosition < 0 || newPosition >= m->fullSize) return 0;

    // Check if the position is occupied
    // N.B: player noise counts as free spaces
    if(m->map[newPosition] != ' ' && m->map[newPosition] != GetNoiseFace(m->player))
        return 0;
    
    return 1;
}

int MovePlayer(MAP m, PLAYER_MOVEMENT move)
{
    int playerMove = CalculatePosition(m->player, move);

    // Exit from game
    if(playerMove < -m->width) return 0;

    if(EvaluatePlayerPosition(m, playerMove))
        ConfirmePosition(m->player);
    else
        RejectPosition(m->player);

    return 1;
}

static void SetGuardOnMap(MAP m, GUARD g)
{
    m->map[GetGuardPosition(g)] = GetGuardFace(g);

    int* guardRange = GetRangePositions(g);
    int range = GetRange(g);

    for(int i = 0; i < range; i++)
    {
        if(guardRange[i] < 0) continue;
        m->map[guardRange[i]] = GetRangeFace(g);
    }
}

static int SetAllGuards(MAP m)
{
    int update = 0;
    for(int i = 0; i < m->totalGuards; i++)
    {   
        update = update || UpdateGuard(m->allGuards[i]);
        SetGuardOnMap(m, m->allGuards[i]);
    }

    return update;
}

void CreateRandomGuards(MAP m, int totalGuards)
{
    m->totalGuards = totalGuards;
    m->allGuards = malloc(totalGuards * sizeof(*m->allGuards));

    for(int i = 0; i < totalGuards; i++)
    {
        int 
            startX = RandomIntFrom0ToMax(m->height), 
            startY = RandomIntFrom0ToMax(m->width), 
            range = RandomInt(1, 5), 
            speed = RandomInt(3, 15);
        GUARD_DIRECTION direction = RandomInt(1, 3);

        m->allGuards[i] = NewGuard(startX, startY, range, direction, speed, m->height, m->width);

        AddFC(m->specialChar, GetGuardFaceColor(m->allGuards[i]));
        AddFC(m->specialChar, GetRangeCol(m->allGuards[i]));
    }
}

static void ResetMap(MAP m)
{
    for(int i = 0; i < m->fullSize; i++)
        m->map[i] = m->baseMap[i];
}

static void INTERNAL_PrintMap(MAP m, int forcePrint)
{
    ResetMap(m);
    int guardUpdate = SetAllGuards(m);
    int playerUpdate = SetPlayerOnMap(m);

    // No update
    if((!guardUpdate && !playerUpdate) || forcePrint) return;

    // Print updated map
    ClearAndHome();

    // Top border
    printf("/");
    for(int i = 0; i < m->width; i++) printf("-");
    printf("\\\n");

    // Map
    for(int r = 0; r < m->height; r++)
    {
        printf("|");
        for(int c = 0; c < m->width; c++)
        {
            char toPrint = m->map[POS(r, c, m->width)];
            FACE_COLOR fcPrint = GetFCFromFace(m->specialChar,  toPrint);

            BackGroundAndText(GetBackgroundColor(fcPrint), GetTextColor(fcPrint));
            printf("%c", toPrint);
            BackGroundAndText(COL_BLACK, COL_NORMAL);
        }
        printf("|\n");
    }

    // Bottom border
    printf("\\");
    for(int i = 0; i < m->width; i++) printf("-");
    printf("/");
}

void PrintMap(MAP m)
{
    INTERNAL_PrintMap(m, 0);
}

int GetMapWidth(MAP m)
{
    return m->width;
}

int GetMapHeight(MAP m)
{
    return m->height;
}

void StartGame(MAP m)
{
    m->thisFrame = clock();
    INTERNAL_PrintMap(m, 1);
}

void UpdateMap(MAP m)
{
    clock_t time = clock();
    if(time - m->thisFrame < m->deltaTime)
        return;

    PrintMap(m);
    m->thisFrame = clock();
}