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

static void SetPlayerOnMap(MAP m)
{
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
}

void AddPlayer(MAP m, PLAYER p)
{
    m->player = p;

    // Set Position
    SetPlayerOnMap(m);

    // Save characteristics
    AddFC(m->specialChar, GetPlayerFaceColor(m->player));
    AddFC(m->specialChar, GetNoise(m->player));
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
        SetGuardOnMap(m, m->allGuards[i]);

        AddFC(m->specialChar, GetGuardFaceColor(m->allGuards[i]));
        AddFC(m->specialChar, GetRangeCol(m->allGuards[i]));
    }
}

static void ResetMap(MAP m)
{
    for(int i = 0; i < m->fullSize; i++)
        m->map[i] = m->baseMap[i];
}

void PrintMap(MAP m)
{
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

    ResetMap(m);
}

int GetMapWidth(MAP m)
{
    return m->width;
}

int GetMapHeight(MAP m)
{
    return m->height;
}