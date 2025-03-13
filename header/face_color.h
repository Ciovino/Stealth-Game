#ifndef FACE_COLOR_H_INCLUDED
#define FACE_COLOR_H_INCLUDED

typedef struct faceColor* FACE_COLOR;
typedef struct fcArray* FC_ARRAY;

typedef enum
{
    COL_BLACK,
    COL_BLUE,
    COL_GREEN,
    COL_CYAN,
    COL_RED,
    COL_MAGENTA,
    COL_BROWN,
    COL_NORMAL, // COL_LIGHT_GRAY
    COL_DARK_GRAY,
    COL_LIGHT_BLUE,
    COL_LIGHT_GREEN,
    COL_LIGHT_CYAN,
    COL_LIGHT_RED,
    COL_PINK,
    COL_YELLOW,
    COL_WHITE,
} COLOR;

// Change terminal appereance
void Textcolor(COLOR);
void BackGroundAndText(COLOR, COLOR);

// Memory managment
FACE_COLOR NewFaceColor(char face, COLOR text, COLOR background);
void FreeFaceColor(FACE_COLOR f);

FC_ARRAY NewFCArray();
void FreeFCArray(FC_ARRAY fca);

// FACE_COLOR operations
char GetFace(FACE_COLOR fc);
COLOR GetColorText(FACE_COLOR fc);
COLOR GetBackgroundColor(FACE_COLOR fc);

// FC_Array operations
void AddFC(FC_ARRAY fca, FACE_COLOR fc);
FACE_COLOR GetFCFromFace(FC_ARRAY fca, char face);

#endif