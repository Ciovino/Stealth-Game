#ifndef FACE_COLOR_H_INCLUDED
#define FACE_COLOR_H_INCLUDED

#include "..\header\Colori.h"

typedef struct faceColor* FACE_COLOR;
typedef struct fcArray* FC_ARRAY;

// Memory managment
FACE_COLOR NewFaceColor(char face, COLOR text, COLOR background);
void FreeFaceColor(FACE_COLOR f);

FC_ARRAY NewFCArray();
void FreeFCArray(FC_ARRAY fca);

// FACE_COLOR operations
char GetFace(FACE_COLOR fc);
COLOR GetTextColor(FACE_COLOR fc);
COLOR GetBackgroundColor(FACE_COLOR fc);

// FC_Array operations
void AddFC(FC_ARRAY fca, FACE_COLOR fc);
FACE_COLOR GetFCFromFace(FC_ARRAY fca, char face);

#endif